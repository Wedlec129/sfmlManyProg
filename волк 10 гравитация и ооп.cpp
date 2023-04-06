#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <math.h>
#include "map.h"
#include "view.h"
#include "mission.h"
using namespace std;
using namespace sf;




////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {
public:

	float dx, dy, x, y, speed;
	int w, h, health;
	bool life;

	bool onGround;

	Texture t;
	Sprite s;

	Player(String Faile, float X, float Y, float W, float H) {

		life = true;
		speed = 0; health = 1000; dx = 0; dy = 0;
		
		onGround = false;

		t.loadFromFile(Faile);
		s.setTexture(t);
		x = X; y = Y;
		w = W;
		h = H;
		s.setTextureRect(IntRect(0, 0, W, H));//-
		s.setOrigin(w / 2, h / 2);
	}



	enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	int playerScore;//эта переменная может быть только у игрока

	

	void control() {
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				state = jump; dy = -0.6; onGround = false;//увеличил высоту прыжка
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
				{
					if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
					if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
				}
				//else { onGround = false; }//надо убрать т.к мы можем находиться и на другой поверхности или платформе которую разрушит враг
			}
	}

	void update(float time)
	{
		control();//функция управления персонажем
		switch (state)//тут делаются различные действия в зависимости от состояния
		{
		case right:dx = speed; break;//состояние идти вправо
		case left:dx = -speed; break;//состояние идти влево
		case up: break;//будет состояние поднятия наверх (например по лестнице)
		case down: dx = 0; break;//будет состояние во время спуска персонажа (например по лестнице)
		case stay: break;//и здесь тоже		
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
		y += dy * time;
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		s.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра

		speed = 0;

		if (health <= 0) { life = false; }
		
		//if (!onGround) { dy = dy + 0.0015*time; }//убираем и будем всегда притягивать к земле

		dy = dy + 0.0015 * time;//постоянно притягиваемся к земле
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
	RenderWindow window(VideoMode(1366, 768), "lol");
	view.reset(FloatRect(0, 0, 840, 580));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации
	setlocale(LC_ALL, "Russian");
	
	

	Player p("images/hero.png", 250, 250, 96, 96);


	float ani = 0;
	Clock clock;
	
	
	Texture mapT;
	mapT.loadFromFile("images/map.png");
	Sprite mapS;
	mapS.setTexture(mapT);



	//




	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;



		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора c левого верхнего угла
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		

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
			
			//
	

		//
		//

		p.update(time);
		window.draw(p.s);
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		

	



		window.display();
	}

	return 0;
}