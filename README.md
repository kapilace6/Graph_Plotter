# Graph Plotter
Graph Plotter is a project that aims to take function input from the user and graphically plot the curve for that function. This project is implemented in C++ using the OpenGL library.
## Installing OpenGL (Ubuntu)
You need to download .deb files for libglui2c2 and libglui-dev.
```shell
sudo apt-get update
sudo apt-get install freeglut3 freeglut3-dev
sudo deb -i [path/to/libglui2c2(.deb)]
sudo deb -i [path/to/libglui-dev(.deb)]
```
## Compiling and Executing
```shell
cc plotter.cpp -lstdc++ -lm -lglut -lGLU -lGL
./a.out
```
## Contributing
Please refer to [CONTRIBUTING.md](https://github.com/utkarsh23/Graph-Plotter/blob/master/CONTRIBUTING.md)
