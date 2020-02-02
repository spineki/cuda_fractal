// Pour lancer un programme et utiliser le profiler, lancer le cmd en admin
// astuce: win + R, cmd, Ctr + Shift + enter
// Ici, f8 build, f9 run, f10 profile


#include "saver.h"
#include <iostream>
#include <math.h>
#include <fstream>


__global__
void mandelbrot(int N, int* z, int W, int H, double xmin, double xmax, double ymin, double ymax, int iter = 100) {

	double c_re, c_im;
	double z_re = 0.0;
	double z_im = 0.0;
	double z_re2, z_im2;

	int i, j;

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;

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
				z[k] = it;
				verif = false;
				break;
			}
		}
		if (verif == true) {
			z[k] = 0;
		}
	}
}


int main(void)
{

	//number of items on each axiss
	int W = 5'000;
	int H = 5'000;
	int N = H * W;

	double xmin = -2.0;
	double xmax = 0.5;
	double ymin = -1.25;
	double ymax = 1.25;

	int iter = 100;

	std::cout << W << " " << H << std::endl;
	int* z, * d_z;

	z = (int*)malloc(N * sizeof(int));
	cudaMalloc(&d_z, N * sizeof(int));

	cudaMemcpy(d_z, z, N * sizeof(double), cudaMemcpyHostToDevice);

	int blockSize = 256;
	int numBlocks = (N + blockSize - 1) / blockSize;

	std::cout << "avant le lancement du kernel" << std::endl;
	mandelbrot << <numBlocks, blockSize >> > (N, d_z, W, H, xmin, xmax, ymin, ymax, iter);
	std::cout << "après le lancement du kernel" << std::endl;
	std::cout << "avant la copie" << std::endl;
	cudaMemcpy(z, d_z, N * sizeof(int), cudaMemcpyDeviceToHost);
	std::cout << "après la copie" << std::endl;




	save(z, W, H);

	/*
	std::ofstream file;
	std::cout << "Début de l'écriture" << std::endl;
	file.open("fractal_parallel.pnm");

	std::cout << "After open" << std::endl;
	file << "P3" << "\n";
	file << W << " " << H << "\n";
	file << iter << std::endl;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			file << "  " << z[i * H + j] << "  " << z[i * H + j] << "  " << z[i * H + j] << "  \n";
		}
	}

	file.close();

	std::cout << "Fin de l'écriture" << "\n";
	cudaFree(d_z);
	free(z);
	std::cout << "Fin des free" << "\n";
	*/
	return 0;
}
