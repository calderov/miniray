# miniray

This project is my own implementation of a raytracer based on the famous book "Ray Tracing in One Weekend" by Peter Shirley, Trevor David Black, and Steve Hollasch.

https://raytracing.github.io/

## Prerequisites

- g++ compiler with C++14 support
- Linux operating system

## Compilation

To compile the project, clone this repository and run the make command in the project's root directory:

```bash
git clone https://github.com/calderov/miniray.git
cd miniray
make
```

This will create the `miniray` executable and ensure the `render` directory exists for output files.

## Running the Project

After compilation, run the raytracer with:

```bash
./miniray
```

The program will render the scene and save the output image(s) as BMP files in the `render/` directory. By default, it renders a single frame (`frame0.bmp`).

## Additional Notes

- The project uses multi-threading for rendering, utilizing all available CPU cores.
- Different scenes can be rendered by modifying the include statement in `src/main.cpp` (e.g., change `#include "scene01.h"` to `#include "scene02.h"`).
- For debugging, use `make debug` to compile with debug flags.