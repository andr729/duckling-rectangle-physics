## Rectangle physics

> [!Note]
> This is a simple, experimental project written in very early, experimental version of Duckling.
> It was created as one of a first experiments with the language.
> See Duckling at [https://duckling.pl/](https://duckling.pl/).

Rectangle physics – rotating, rigid, 2D rectangle physics simulation written in very early, experimental version of Duckling.

The physics itself is not actually correct, but it was made good enough to look natural.

## Demo

https://github.com/user-attachments/assets/fd6af2b0-561f-4cf5-a270-70bcdb845535


https://github.com/user-attachments/assets/a30ad692-8ff7-4ec1-914b-63a22ca6b66d


## Building and running

The project was created with a very old version of the Duckling compiler, which is required to build this project.
Newer versions of the compiler *might* be able to build this project, but it is not guaranteed.

Dependencies:
* SFML

To build and run the project, you can use the following commands:

```bash
./build_cpp.sh   # Builds simple C++ sfml port
./build.sh -O 3  # Builds the Duckling code (with optimizations)
./run.sh         # Runs the built Duckling code
```


