#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;






int main() {
	setlocale(LC_ALL, "Russian");
	RenderWindow window(sf::VideoMode(400, 400), "lol");

	Image lol;
	lol.loadFromFile("images/hero.png");
	

	Texture kek;
	kek.loadFromImage(lol);

	Sprite qwerty;
	qwerty.setTexture(kek);
	qwerty.setTextureRect(IntRect(0, 192, 96, 96)); ///x y ж в
	qwerty.setPosition(150, 150);
	
	float ani = 0;

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)window.close();
		};

		window.clear(Color(0, 128, 129));


		window.draw(qwerty);


		if (Keyboard::isKeyPressed(Keyboard::D)) {
			qwerty.move(0.1, 0);

			ani = ani + 0.005; //скорость анимации
			if (ani > 3) ani = 0;
			qwerty.setTextureRect(IntRect(96 * int(ani), 192, 96, 96));  //1 ширина точ нач.. коар спрайт..
			
		};

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			qwerty.move(-0.1, 0);

			ani = ani + 0.005; //скорость анимации
			if (ani > 3) ani = 0;
			qwerty.setTextureRect(IntRect(96 * int(ani), 96, 96, 96));  //1 ширина точ нач.. коар спрайт..
		};

		if (Keyboard::isKeyPressed(Keyboard::W)) {
			qwerty.move(0, -0.1);

			ani = ani + 0.005; //скорость анимации
			if (ani > 3) ani = 0;
			qwerty.setTextureRect(IntRect(96 * int(ani), 300, 96, 96));  //1 ширина точ нач.. коар спрайт..
		};

		if (Keyboard::isKeyPressed(Keyboard::S)) {
			qwerty.move(0, 0.1);

			ani = ani + 0.005; //скорость анимации
			if (ani > 3) ani = 0;
			qwerty.setTextureRect(IntRect(96 * int(ani), 11, 96, 96));  //1 ширина точ нач.. коар спрайт..
		};

		//if (Keyboard::isKeyPressed(Keyboard::R))  qwerty.setColor(color(255, 0, 0));


		window.display();
	}



	return 0;
}
