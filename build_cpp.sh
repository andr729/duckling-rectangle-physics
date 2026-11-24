mkdir -p build/c
g++-14 -c -std=c++23 \
    -O3 \
    ./c_port/sfml_port.cpp \
    -o build/c/sfml_port.o
ar rcs build/c/libsfml_port.a build/c/sfml_port.o
