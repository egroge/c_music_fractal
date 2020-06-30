# C Music Fractal 

This is a C group project extension written in first year of Imperial College London. 
It uses opengl to render fractals on the GPU, and has them 'dance' in real time to music files passed in.

It was written and tested on Ubuntu linux, for opengl version 3.3

Command line arguments:
```bash
./main <PATH TO SHADER TO USE> <PATH TO WAV FILE TO USE>
```

This project has dependencies.

 - SDL2 is needed for this project. If You're on Debian/Ubuntu you can install SDL with:

```
sudo apt-get install libsdl2-dev
```
 - openGL
```
sudo apt-get install libgl-dev
sudo apt-get install libglew-dev
```
Note: the power_n shader does not work as a command line argument, since it takes different arguments
