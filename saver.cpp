#include "saver.h"
#include <iostream>
#include <SFML/Graphics.hpp>


void save(int array[], int W, int H) {

	std::cout<< "avant la creation de l'image\n";
	sf::Image image;
	image.create(W, H, sf::Color::Black);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			sf::Color color = image.getPixel(i, j);
			color.r = array[i * W + j];
			color.b = array[i * W + j];
			color.g = array[i * W + j];
			image.setPixel(i, j, color);
		}
	}

	std::cout << "avant l'enregistrement de l'image" << std::endl;
	if (!image.saveToFile("E:/Programmation/CUDA/test_somme/result.png")) {
		std::cout << "Impossible d'enregistrer!!" << std::endl;
	}

}
