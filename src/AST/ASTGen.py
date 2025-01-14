import sys, datetime, json
from pathlib import Path

def header(filesource):
    result = []
    result.append("/*")
    result.append(" * This file was automatically generated by gen_parse.py")
    result.append(" * on " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    result.append(" * from " + filesource.resolve().name)
    with open(filesource, "r") as f:
        for line in f.readlines():
            result.append(" *  " + line.strip())
    result.append(" */")
    headers = [
        "#pragma once",
        "",
        "#include <any>",
        "#include <memory>",
        "#include <utility>  // std::move",
        "#include \"token.hpp\"",
    ]

    return result + headers

def get_gram_dict(filesource):
    if not filesource.is_file():
        print(f"Input file '{filesource.resolve()}' does not exist" )
        sys.exit(64)

    result = {}
    with open(filesource, "r") as f:
            for line in f.readlines():
                node, attributes = line.strip().split(":")
                attributes = attributes.strip().split(",")
                att_dict = {}
                for att in attributes:
                    value, key = att.strip().split(" ")
                    if value == "any":
                        value = "std::any"
                    elif "*" in value:
                        value = value.replace("*", "")
                        value = "std::shared_ptr<" + value + ">"
                    att_dict[key] = value

                result[node.strip()] = att_dict
    return result

def forward_declaration(gram):
    result = [] 
    for node in gram.keys():
        result.append(f"struct {node};")
    return result

def define_visitor(base_class, gram):
    result = []
    result.append(f"struct {base_class}Visitor {{")
    for node in gram.keys():
        result.append(f"  virtual std::any visit{node}{base_class}(std::shared_ptr<{node}> {base_class.lower()}) = 0;")
    result.append(f"  virtual ~" + base_class + "Visitor() = default;")
    result.append("};")
    return result

def define_interface():
    result = []
    result.append(f"struct {base_class} {{")
    result.append(f"  virtual std::any accept({base_class}Visitor& visitor) = 0;")
    result.append("};")
    return result

def define_type(gram):
    result = []
    for node, att in gram.items():
        result.append(f"struct {node}: {base_class}, public std::enable_shared_from_this<{node}>  {{")

        input = []
        for key, value in att.items():
            input.append(f"{value} {key}")
        input = ", ".join(input)
        result.append(f"  {node}({input})") 
        const = []
        for key in att.keys():
            const.append(f"{key}{{std::move({key})}}")

        const = ", ".join(const)
        result.append("    : " + const)
        result.append("  {}")
        result.append("")
        result.append("  std::any accept(" + base_class + "Visitor& visitor) override {")
        result.append("    return visitor.visit" + node + base_class + "(shared_from_this());")
        result.append("  }")
        result.append("")

        for key, value in att.items():
            result.append(f" const {value} {key};")
        result.append("};")
        result.append("")
    return result

def generate_file(filesource, base_class, gram_dict):
    output = []
    output += header(filesource) + [""]
    output += forward_declaration(gram_dict)+ [""]
    output += define_visitor(base_class, gram_dict)+ [""]
    output += define_interface()+ [""]
    output += define_type(gram_dict)
    return output


if __name__ == '__main__':
    if len(sys.argv) == 2:
        outdir = Path("../../src")
        print(f"Using default output directory '{outdir.resolve()}'" )
    elif len(sys.argv) == 3:
        outdir = Path(sys.argv[2])
        if not outdir.is_dir():
            print(f"Output directory '{outdir}' does not exist" )
            sys.exit(64)
    else:
        print("Usage: gen_parse.py <input_file> <output_dir>")
        sys.exit(64)

    filesource = Path(sys.argv[1])

    base_class = filesource.name.split(".")[0]
    gram_dict = get_gram_dict(filesource)

    output = "\n".join(generate_file(filesource, base_class, gram_dict))
    
    outfile = outdir/ f"{base_class}.hpp"
    if outfile.is_file():
        print(f"Output file '{outfile}' already exists. Overwriting." )
        # sys.exit(64)

    with open(outfile, "w") as f:           
        print(f"Generating output to '{outfile}'" )
        f.write(output)
     
