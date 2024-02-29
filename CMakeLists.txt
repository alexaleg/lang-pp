cmake_minimum_required(VERSION 3.5)
project(langpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/repl.cpp
)

#Boost stuff
set(BOOST_REQUIRED_COMPONENTS  unit_test_framework)
set(BOOST_MIN_VERSION 1.74.0)
# Find Boost
find_package(Boost  
    REQUIRED ${BOOST_REQUIRED_COMPONENTS} )


add_executable(langpp ${SOURCES})

target_include_directories(langpp PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(langpp PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${EXECUTABLE} ${Boost_LIBRARIES})

enable_testing()
## Add a testing executable
add_executable(testScanner test/unit/testScanner.cpp)

target_include_directories(testScanner PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(testScanner
    Boost::unit_test_framework
)

target_compile_definitions(testScanner
    PRIVATE
        BOOST_TEST_DYN_LINK
)

add_test(test_all testScanner)
