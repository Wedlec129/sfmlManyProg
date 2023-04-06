//1

#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
using namespace std;
using namespace sf;


class Player { 
private:
	float x, y, dx, dy=0;

public:
	float speed = 0; 
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

	float getplayercoordinateX() {	//этим методом будем забирать координату Х	
		return x;
	}
	float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
		return y;
	}


	void getplayercoordinateforview(float x, float y) { //функция для считывания координат игрока


		view.setCenter(x + 100, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте


	}

};


int main()
{
	RenderWindow window(VideoMode(640, 480), "lol");
	view.reset(FloatRect(0, 0, 640, 480));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации
	

	Player p("images/hero.png", 250, 250, 96, 96);


	float ani = 0;
	Clock clock;

	
	Texture mapT;
	mapT.loadFromFile("images/map.png");
	Sprite mapS;
	mapS.setTexture(mapT);


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



		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  mapS.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  mapS.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((TileMap[i][j] == '0')) mapS.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


				mapS.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(mapS);//рисуем квадратики на экран
			}

		






		p.move(time);
		window.draw(p.s);
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

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




//2


#pragma once
#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 40;//размер карты ширина 

using namespace sf;

String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0   s                                  0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};


//3

#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


View view;
View getplayercoordinateforview(float x, float y) { //функция для считывания координат игрока


	view.setCenter(x + 100, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте

	return view;
};


