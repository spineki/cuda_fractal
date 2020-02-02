#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/WINDOW/Mouse.hpp>


#include "kernel.h"


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



int main() {

	// limits of the picture
	const int W = 8'00;
	const int H = 8'00;
	const int N = H * W;

	double xmin = -2.0;
	double xmax = 0.5;


	double ymin = -1.25;
	double ymax = 1.25;


	double f = 0.10; // zoom factor

	int iter = 1000; // max number of iteration per pixel
	int* z;
	z = (int*)malloc(N * sizeof(int));

	// graphic part

	sf::RenderWindow window(sf::VideoMode(W, H), "Cuda_fractale");
	window.setFramerateLimit(25);

	sf::Image image;
	image.create(W, H, sf::Color::Black);

	sf::Texture texture;
	texture.create(W, H);

	sf::Sprite sprite;

	bool update = true;


	while (window.isOpen())
	{
		Sleep(16);
		sf::Event event;
		if(window.pollEvent(event))
		{

			double height = ymax - ymin;
			double width = xmax - xmin;

			if (event.type == sf::Event::MouseButtonPressed) {
				update = true;
				auto pos = sf::Mouse::getPosition(window);
				
				double ncx = pos.x * (xmax - xmin)/W + xmin;
				double ncy = pos.y * (ymax - ymin)/H + ymin;
				double cx = (xmin + xmax) / 2;
				double cy = (ymin + ymax) / 2;

				double transx = ncx - cx;
				double transy = cy-ncy;

				std::cout << transx << " " << transy<< std::endl;
				xmin += transx;
				xmax += transx;
				ymin += transy;
				ymax += transy;

				if (event.mouseButton.button == sf::Mouse::Right) {
					std::cout << "click droit\n";
					xmin -= width / 10.f;
					xmax += width / 10.f;
					ymin -= height / 10.f;
					ymax += height / 10.f;
				}

				else if (event.mouseButton.button == sf::Mouse::Left) {
					std::cout << "click gauche\n";
					xmin += width / 10.f;
					xmax -= width / 10.f;
					ymin += height / 10.f;
					ymax -= height / 10.f;
				}
			}


			if (event.type == sf::Event::KeyPressed)
			{
				update = true;


				if (event.key.code == sf::Keyboard::Up)
				{
					
					ymin += height / 10.f;
					ymax += height / 10.f;
				}

				else if (event.key.code == sf::Keyboard::Down)
				{
	
					ymin -= height / 10.f;
					ymax -= height / 10.f;
				}


				else if (event.key.code == sf::Keyboard::Left)
				{

					xmin -= width / 10.f;
					xmax -= width / 10.f;
				}

				else if (event.key.code == sf::Keyboard::Right)
				{

					xmin += width / 10.f;
					xmax += width / 10.f;
				}

				else if (event.key.code == sf::Keyboard::Enter)
				{
					xmin += width / 10.f;
					xmax -= width/10.f;
					ymin += height / 10.f;
					ymax -= height / 10.f;
				}

			}


			if (event.type == sf::Event::Closed){
				window.close();
			}

			if (update) {
				// generating a new frame
				newFrame(z, W, H, xmin, xmax, ymin, ymax, iter);

				for (int i = 0; i < H; i++) {
					for (int j = 0; j < W; j++) {
						sf::Color color = image.getPixel(i, j);
						color.r = z[i * W + j];
						color.b = z[i * W + j];
						color.g = z[i * W + j];
						image.setPixel(i, j, color);
					}
				}

				texture.update(image);
				sprite.setTexture(texture);
				sprite.setOrigin(W / 2.f, H / 2.f);
				sprite.setPosition(sf::Vector2f(W / 2.f, H / 2.f));
				sprite.setRotation(-90.f);

				//window.clear();
				window.draw(sprite);
				window.display();
				std::cout << "update" << std::endl;
				update = false;
			}

		}


	}



	// save(z, W, H);
	free(z);


	return 0;
}
