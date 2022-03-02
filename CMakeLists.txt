
cmake_minimum_required(VERSION 3.22)

project(spelling)

add_executable(
    spelling
    src/main.cpp
    src/tokenizer.cpp
    src/wordlist.cpp
    )

file(COPY data/demo.txt DESTINATION data)

target_compile_features(spelling PRIVATE cxx_std_17)

