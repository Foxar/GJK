<img src="https://lh3.googleusercontent.com/pw/ACtC-3fVVoNCg6-QCoL-sOvWEhs6GVJjOTs1_XlO9MtpmLoUReclt6OwY2sXtLTWWD8HxfA_llXTIEfkIROoijxLlTaHmgCKCZG0jR4VZZH9V6tC3KT93qziucu2VZBrDbdapO9_CSo1LDYTpwIWmsoTisMx=w1024-h423-no?authuser=0" height=200>

# GJK






Gilbert–Johnson–Keerthi distance algorithm is a method for determining whether two convex shapes intersect between each other, through usage of Minkowski's difference and 'support functions'.

This repository holds a rudimentary OpenGL renderer, as well as a simple implementation of the algorithm.

# Dependancies

The project requires following dependancies:

- freeglut3
- glm
- glew
- glad
- glfw


# Installation

Warning: The project has been developed on Windows, without planned support for 
After installing the appropriate dependancies, modification to the makefile is necessary:

`g++ $(OBJS) -IC:\mingw_dev_lib\glfw\include` - Replace the path to your installation path of glfw, as well as other dependancies reference in the makefile.

# Compiling

Run `make` inside the project folder
