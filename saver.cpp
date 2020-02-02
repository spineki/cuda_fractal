#include "saver.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/WINDOW/Mouse.hpp>


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

	sf::RenderWindow window(sf::VideoMode(W, H), "Fractale");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	texture.create(W, H);
	texture.update(image);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setOrigin(W / 2.f, H / 2.f);
	sprite.setPosition(sf::Vector2f(W / 2.f, H / 2.f));
	sprite.setRotation(-90.f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				std::cout << "click" << std::endl;
				auto pos = sf::Mouse::getPosition(window);
				std::cout << pos.x <<" "<< pos.y<< std::endl;
			}


			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}
	/*
	std::cout << "avant l'enregistrement de l'image" << std::endl;
	if (!image.saveToFile("E:/Programmation/CUDA/test_somme/result.png")) {
		std::cout << "Impossible d'enregistrer!!" << std::endl;
	}*/

}
