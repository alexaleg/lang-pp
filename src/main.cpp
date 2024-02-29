#include <fstream>
#include <iostream>
#include <string>

#include "repl.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    cout << "Initializing REPL " << endl;
    main_loop();
  } else {
    ifstream file(argv[1]);
    if (!file.is_open()) {
      cerr << "Could not open file: " << argv[1] << endl;
      return 1;
    }
    string line;
    cout << "Found file: " << argv[1] << endl;
    while (getline(file, line)) {
      cout << line << endl;
    }

    file.close();
  }
  return 0;
}
