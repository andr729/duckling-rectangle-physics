# no incremental for now

duckc \
    compile_package game \
    -n game \
    -a ./build/duck/ \
    --linker "g++-14" \
    --additional-link-options "./build/c/sfml_port.o  -lsfml-graphics -lsfml-window -lsfml-system" \
    --no-incremental \
    "$@"
