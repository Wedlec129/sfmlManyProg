#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


class Player { 
public:
	float x, y, dx, dy, speed = 0; 
	int dir = 0;
	
	Texture t;
	Sprite s;

	Player (String Faile, float X, float Y, float W, float H) {  
		
		
		
		t.loadFromFile(Faile);
		s.setTexture(t);
		x = X; y = Y;
		s.setTextureRect(IntRect(0, 0, W, H));  
	}



	void move(float time) 
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;
		case 1: dx = -speed; dy = 0;   break;
		case 2: dx = 0; dy = speed;   break;
		case 3: dx = 0; dy = -speed;   break;
		}

		x =x+ dx * time;
		y =y+ dy * time;

		speed = 0;
		s.setPosition(x, y); 
	}
};


int main()
{
	RenderWindow window(VideoMode(640, 480), "lol");

	

	Player p("images/hero.png", 250, 250, 96, 96);


	float ani = 0;
	Clock clock;


	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();


		p.move(time);
		window.draw(p.s);

		if (Keyboard::isKeyPressed(Keyboard::D)) {
			p.dir = 0; p.speed = 0.1;

			ani = ani + 0.005 * time;
			if (ani > 3) ani = 0;
			p.s.setTextureRect(IntRect(96 * int(ani), 192, 96, 96));
		};
		
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			p.dir = 1; p.speed = 0.1;

			ani=ani+ 0.005 * time;
			if (ani > 3) ani = 0;
			p.s.setTextureRect(IntRect(96 * int(ani), 96, 96, 96)); 
		}

		if (Keyboard::isKeyPressed(Keyboard::W)) {
			p.dir = 3; p.speed = 0.1;

			ani = ani + 0.005 * time;
			if (ani > 3) ani = 0;
			p.s.setTextureRect(IntRect(96 * int(ani), 288, 96, 96));
		}

		if (Keyboard::isKeyPressed(Keyboard::S)) {
			p.dir = 2; p.speed = 0.1;

			ani = ani + 0.005 * time;
			if (ani > 3) ani = 0;
			p.s.setTextureRect(IntRect(96 * int(ani), 0, 96, 96));
		}


		
		window.display();
	}

	return 0;
}