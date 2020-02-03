#include <iostream>
#include <windows.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/WINDOW/Mouse.hpp>


#include "kernel.h"


/*
std::cout << "avant l'enregistrement de l'image" << std::endl;
if (!image.saveToFile("E:/Programmation/CUDA/test_somme/result.png")) {
	std::cout << "Impossible d'enregistrer!!" << std::endl;
}*/

int main() {

	// Welcoming message

	std::cout << " Welcome to the cuda-fractal explorer!\n" << "Here are some controls:\n"
		<< "left arrow: go to left\n"
		<< "right arrow: got to right\n"
		<< "up arrow: go to the top\n"
		<< "down arrow: go to the bottom\n"
		<< "enter: zoom in\n"
		<< "space: zoom out\n"
		<< "\n You can also use your mouse. Left click were you want to zoom in, Right Click where you want to zoom out"
		<< "\n In order to save the current picture, press the 'p' key (like print, yep). the file will be saved in the executable folder with the current coordinate"
		<< "\n Have fun!!\n ************" << std::endl;


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

				xmin += transx;
				xmax += transx;
				ymin += transy;
				ymax += transy;

				if (event.mouseButton.button == sf::Mouse::Right) {
					xmin -= width / 10.f;
					xmax += width / 10.f;
					ymin -= height / 10.f;
					ymax += height / 10.f;
				}

				else if (event.mouseButton.button == sf::Mouse::Left) {
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

				else if (event.key.code == sf::Keyboard::Space)
				{
					xmin -= width / 10.f;
					xmax += width / 10.f;
					ymin -= height / 10.f;
					ymax += height / 10.f;
				}

				else if (event.key.code == sf::Keyboard::P) {
					std::string name = std::to_string(xmin) + "_" + std::to_string(xmax) + "_" + std::to_string(ymin) + "_" + std::to_string(ymax)+".png";
					std::cout << "Saving...";
					if (!image.saveToFile(name.c_str())) {
						std::cout << "Error" << std::endl;
					}
					else {
						std::cout << "done!" << std::endl;
					}
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
						color.r = z[i * W + j] * 1;
						color.b = z[i * W + j] * 2;
						color.g = z[i * W + j] * 3;
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
				update = false;
			}

		}


	}



	// save(z, W, H);
	free(z);


	return 0;
}
