#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "map.h"
#include "view.h"
using namespace std;
using namespace sf;


class Player { 
private:
	float x, y, dx , w ,h , dy=0;

public:
	float speed = 0; 
	int dir = 0;
	

	int ball=0;

	int health =100;
	bool life=true;


	Texture t;
	Sprite s;

	Player (String Faile, float X, float Y, float W, float H) {  
		
		
		
		t.loadFromFile(Faile);
		s.setTexture(t);
		x = X; y = Y;
		w = W;
		h = H;
		s.setTextureRect(IntRect(0, 0, W, H));  
	}



	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;
		case 1: dx = -speed; dy = 0;   break;
		case 2: dx = 0; dy = speed;   break;
		case 3: dx = 0; dy = -speed;   break;
		}

		x = x + dx * time;
		y = y + dy * time;

		speed = 0;
		s.setPosition(x, y);

		if (health <= 0)life = false;
	};

	float getplayercoordinateX() {	//этим методом будем забирать координату Х	
		return x;
	}

	float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
		return y;
	}


	void getplayercoordinateforview(float x, float y) { //функция для считывания координат игрока


		view.setCenter(x + 100, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте


	}



	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					ball++;
					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}

				if (TileMap[i][j] == 'f') {
					health = health- 40;//если взяли ядовитейший в мире цветок,то переменная health=health-40;
					TileMap[i][j] = ' ';//убрали цветок
				}

				if (TileMap[i][j] == 'h') {
					health = health+ 20;//если взяли сердечко,то переменная health=health+20;
					TileMap[i][j] = ' ';//убрали сердечко
				}


			}
	}



};

class text {
public:

	Font font;
	Text txt;
	ostringstream chislo;

	float x, y = 0;

	text(string font,string shribe,int chislo,float x, float y) {

		
		this->font.loadFromFile("font/" + font);

		txt.setFont(this->font);
		txt.setCharacterSize(20);
		txt.setPosition(view.getCenter().x + x, view.getCenter().y + y); //место текста
		this->chislo << chislo;

		txt.setString(shribe+this->chislo.str());

		this->x = x;
		this->y = y;

	}

	void sleditCam() {
		txt.setPosition(view.getCenter().x + x, view.getCenter().y + y); //место текста
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
			if (event.type == sf::Event::Closed || (Keyboard::isKeyPressed(Keyboard::Escape)))
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
				if ((TileMap[i][j] == 'f')) mapS.setTextureRect(IntRect(96, 0, 32, 32));//добавили цветок
				if ((TileMap[i][j] == 'h')) mapS.setTextureRect(IntRect(128, 0, 32, 32));//и сердечко

				mapS.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(mapS);//рисуем квадратики на экран
			}

		//взаимодействие с картой 
		p.interactionWithMap();


		//баллы
		text bal("bahnschrift.ttf", "ball : ", p.ball, -320, -90);
		bal.sleditCam();
		window.draw(bal.txt);
		//
		//жизньи
		text Hp("bahnschrift.ttf", "HP : ", p.health, -320, -120);
		Hp.sleditCam();
		window.draw(Hp.txt);


		p.update(time);
		window.draw(p.s);
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());





		


		

		if (p.life==true) {

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				p.dir = 0; p.speed = 0.1;

				ani = ani + 0.005 * time;
				if (ani > 3) ani = 0;
				p.s.setTextureRect(IntRect(96 * int(ani), 192, 96, 96));





			};

			if (Keyboard::isKeyPressed(Keyboard::A)) {
				p.dir = 1; p.speed = 0.1;

				ani = ani + 0.005 * time;
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

		};
		
		window.display();
	}

	return 0;
}