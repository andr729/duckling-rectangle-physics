## Rectangle physics

> [!Note]
> This is a simple, experimental project written in early, experimental version of Duckling (currently migrated to Duckling `dev-weekly@57f76bb9b`).
> It was initially created as one of a first experiments with the language.
> See Duckling at [https://duckling.pl/](https://duckling.pl/).

Rectangle physics – rotating, rigid, 2D rectangle physics simulation written in Duckling.

The physics itself is not actually correct, but it was made good enough to look natural.

## Demo

https://github.com/user-attachments/assets/fd6af2b0-561f-4cf5-a270-70bcdb845535


https://github.com/user-attachments/assets/a30ad692-8ff7-4ec1-914b-63a22ca6b66d


## Building and running

Dependencies:
* Duckling toolchain (`duck` and `duckc`)
* SFML 2 (e.g. 2.6; SFML 3 is not supported by the C++ port)
* `g++-14`

To build and run the project, you can use the following commands:

```bash
./build_cpp.sh        # Builds simple C++ sfml port (build/c/libsfml_port.a)
duck build --release  # Builds the Duckling code (with optimizations)
duck run --release
```
