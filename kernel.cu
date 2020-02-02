// Pour lancer un programme et utiliser le profiler, lancer le cmd en admin
// astuce: win + R, cmd, Ctr + Shift + enter
// Ici, f8 build, f9 run, f10 profile


#include "kernel.h"
#include <iostream>
#include <math.h>
#include <fstream>


__global__
void mandelbrot(int N, int* z, int W, int H, double xmin, double xmax, double ymin, double ymax, int iter) {

	double c_re, c_im;
	double z_re = 0.0;
	double z_im = 0.0;
	double z_re2, z_im2;

	int i, j;

	const int index = blockIdx.x * blockDim.x + threadIdx.x;
	const int stride = blockDim.x * gridDim.x;

	int k;
	for (k = index; k < N; k += stride) {
		i = k / W;
		j = k % W;
		c_re = xmin + (double)(xmax - xmin) * (double)j / W;
		c_im = ymin + (double)(ymax - ymin) * (double)i / H;

		bool verif = true;
		for (int it = 0; it < iter; ++it) {
			z_re2 = z_re * z_re;
			z_im2 = z_im * z_im;

			z_im = 2 * z_re * z_im + c_im;
			z_re = z_re2 - z_im2 + c_re;

			if (z_re2 + z_im2 > 4.0) {
				z[k] = it *255 / iter;
				verif = false;
				break;
			}
		}
		if (verif == true) {
			z[k] = 0;
		}
	}
}



void newFrame(int* z, int W, int H, double xmin, double xmax, double ymin, double ymax , int iter =255) {
	const unsigned int N = H * W;

	// alloc
	int* d_z; // limit à faire en passage par référence
	cudaMalloc(&d_z, N * sizeof(int));
	cudaMemcpy(d_z, z, N * sizeof(double), cudaMemcpyHostToDevice);

	int blockSize = 256;
	int numBlocks = (N + blockSize - 1) / blockSize;
	mandelbrot <<<numBlocks, blockSize >>> (N, d_z, W, H, xmin, xmax, ymin, ymax, iter);
	cudaMemcpy(z, d_z, N * sizeof(int), cudaMemcpyDeviceToHost);

	//free (attention le z n'a pas été free)
	cudaFree(d_z);
}