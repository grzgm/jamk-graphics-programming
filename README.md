# JAMK Graphics Programming

This repository is for the subject Graphics Programming at JAMK University. Subject teaches:

- geometric transformations,
- shading models,
- rasterization,
- lighting,
- texturing,
- object-oriented graphics programming,
- graphics pipelines,
- APIs such as OpenGL.

## Technologies

- C++
- GLSL

## Compilation

To compile the solutions run command `make` in the corresponding folder. Each solution includes its own `Makefile`, specifying the necessary dependencies. The compilation process involves compiling `main.cpp` and `TheApp.cpp` with debugging enabled, linking the resulting object files into an executable named `app`, and cleaning up the object files afterward.

## Repository Contents

`assignment` folders contain solutions to the University's assignments.
`lesson` folders contain lesson materials and free coding based on lesson content.

- `lesson01` - coordinate system, vertex data structure, rendering quad, 3D space transformations, adjusting face colour, multicolour tetrahedron.
- `lesson02` - scene graph.
- `lesson03` - gouraud shading.
- `lesson04` - 3D object material colours: Ambient, Diffuse, Emissive, Specular.
- `lesson05` - lighting with phong shading, collision system.
- `lesson06` - multitexturing.
- `lesson07` - blending.

- `assignment-1` - rotating triangle with interpolated colours.
- `assignment-2` - transparent texturing with UV coordinates.
- `assignment-3` - parenting objects with scenegraph and transformations based on the local coordinates from Model Matrix.
- `assignment-4` - multitextured box with specular effect only on metallic parts.
