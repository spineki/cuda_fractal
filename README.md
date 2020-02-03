Cuda_fractal
==============
This project was first designed to try the raw power of cuda. A mandelbrot set is computated every time we press a key (or a mouse button).
A copy of the screen can be saved by pressing the "P" key.

# Files
cuda_fractal.cpp: the main file that handle windows, keys and picture update

kernel.cu: The cuda file were the kernel is written. It is call by the main function in cuda_fractal.cpp/


# Requirements
The nvcc NVIDIA compiler.
The SFML 2.X library => the .dll files related: sfml-graphics-2.dll, sfml-system-2.dll, sfml-window-2.dll in the executable folder.


# TODO

Find a way to keep the device pointer alive and not recreate the array in the GPU memory every time a frame is computated.
Make a config file to avoid hardcoded parameters (number of pixels, frame size...)
Try some others fractals!?
