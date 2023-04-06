#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <math.h>
#include <list>
#include <vector>
#include "level.h"
#include "view.h"
using namespace std;
using namespace sf;



class Entity {
public:
	vector<Object> obj;//вектор объектов карты с котрыми будет взаимодействовать

	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер дл€ будущих целей
	int w, h, health;
	bool life, onGround;
	Texture t;
	Sprite s;
	String name;//враги могут быть разные, мы не будем делать другой класс дл€ врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false;
		t.loadFromImage(image);
		s.setTexture(t);
		s.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//ф-ци€ получени€ пр€моугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ци€ нужна дл€ проверки столкновений 
	}



	

};





class Player :public Entity {
public:

	enum { left, right, up, down, jump, stay } state;//добавл€ем тип перечислени€ - состо€ние объекта

	int playerScore;


	Player(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lvl.GetAllObjects();//инициализируем.получаем все объекты дл€ взаимодействи€ персонажа с картой
		if (name == "Player1") {
			s.setTextureRect(IntRect(4, 19, w, h));
		}



	}








	void control() {
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::A)) {//а именно лева€
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right; speed = 0.1;
			}

			if (((Keyboard::isKeyPressed(Keyboard::W)) || (Keyboard::isKeyPressed(Keyboard::Space))) && (onGround)) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				state = jump; dy = -0.6; onGround = false;//увеличил высоту прыжка
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ци€ проверки столкновений с картой
	{

		for (int i = 0; i < obj.size(); i++)//проходимс€ по объектам
			if (getRect().intersects(obj[i].rect))//провер€ем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили преп€тствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }                   /////вниз/типо верх того обьекта - высота игрока игрока
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }               /////вверх//типо верх того обьекта + высота того обьекта
					if (Dx > 0) { x = obj[i].rect.left - w; }                                      /////в право///типо левый край того обьекта - ширина игрока
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }                   /////в лево//типо левый край того обьекта + ширинатого обьекта
				} 
			}


		

	}

	void update(float time) 
	{
		control();//функци€ управлени€ персонажем
		switch (state)//тут делаютс€ различные действи€ в зависимости от состо€ни€
		{
		case right:dx = speed; break;//состо€ние идти вправо
		case left:dx = -speed; break;//состо€ние идти влево
		case up: break;//будет состо€ние подн€ти€ наверх (например по лестнице)
		case down: dx = 0; break;//будет состо€ние во врем€ спуска персонажа (например по лестнице)
		case stay: break;//и здесь тоже		
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по ’
		y += dy * time;
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		s.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра

		speed = 0; //чтобы спид не был посто€нным

		if (health <= 0) { life = false; }



		dy = dy + 0.0015 * time;//посто€нно прит€гиваемс€ к земле
	}

	float getplayercoordinateX() {	//этим методом будем забирать координату ’	
		return x;
	}

	float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
		return y;
	}


	void getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока


		view.setCenter(x + 100, y); //следим за игроком, передава€ его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте


	}






};

class Enemy :public Entity {
public:
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты дл€ взаимодействи€ врага с картой
		if (name == "EasyEnemy") {
			s.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;//даем скорость.этот объект всегда двигаетс€
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ци€ проверки столкновений с картой
	{

		for (int i = 0; i < obj.size(); i++)//проходимс€ по объектам
			if (getRect().intersects(obj[i].rect))//провер€ем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили преп€тствие
				{
					//if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				//	if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; s.scale(-1, 1); } //типо если мы шли в право то ,, левый край того обьекта с которым столкнулись - ширина нашего врага ,, DX отрицательный и отражаем врага по x
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; s.scale(-1, 1); }
				}
			}



	}

	void update(float time) 
	{
		if (name == "EasyEnemy") {//дл€ персонажа с таким именем логика будет такой


			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по ’
			x += dx * time;
			s.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(1366, 768), "lol");
	view.reset(FloatRect(0, 0, 840, 580));//размер "вида" камеры при создании объекта вида камеры. (потом можем мен€ть как хотим) „то то типа инициализации
	setlocale(LC_ALL, "Russian");



	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Image enimyImages;
	enimyImages.loadFromFile("images/shamaich.png");


	Level lvl;//создали экземпл€р класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	//Object easyEnemyObject = lvl.GetObject("easyEnemy");//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него





	list<Enemy*>  easyEnemy;//создаю список, сюда буду кидать объекты.например врагов.
	
	vector<Object> e = lvl.GetObjects("EasyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе

	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		easyEnemy.push_back(new Enemy(enimyImages, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));//и закидываем в список всех наших врагов с кар





	
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//передаем координаты пр€моугольника player из карты в координаты нашего игрока
	//Enemy e(enimyImages, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);//передаем координаты пр€моугольника easyEnemy из карты в координаты нашего врага



	float ani = 0;
	Clock clock;


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






		/////////////////////////////–исуем карту/////////////////////
		
		lvl.Draw(window);

		/////////////
	
		//обновл€ем и рисуем главного геро€
		p.update(time);
		window.draw(p.s);


		//мы в тот лист можем кинуть всех врагов и потои просто провер€ть их по имени 
		//просто обновл€ем и рисуем + удал€ем
		for (auto it = easyEnemy.begin(); it != easyEnemy.end();)//говорим что проходимс€ от начала до конца
		{
			
			//обновл€ем
			(*it)->update(time);//вызываем ф-цию update дл€ всех объектов (по сути дл€ тех, кто жив)

			//рисуем
			window.draw((*it)->s);

			//если врага сдог то удол€ем его
			if ((*it)->life == false) { it=easyEnemy.erase(it);  }// если этот объект мертв, то удал€ем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка

		}



		//типо дл€ убийств
		for (auto it = easyEnemy.begin(); it != easyEnemy.end(); it++)//проходимс€ по эл-там списка
		{
			if ((*it)->getRect().intersects(p.getRect()))//если пр€моугольник спрайта объекта пересекаетс€ с игроком
			{
				if ((*it)->name == "EasyEnemy") {//и при этом им€ объекта EasyEnemy,то..

					//если мы падали на него
					if ((p.dy > 0) && (p.onGround == false)) { (*it)->dx = 0; p.dy = -0.2; (*it)->health = 0; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
					else {


						//если мы его задели не в прыжке
						p.health -= 5;	//иначе враг подошел к нам сбоку и нанес урон


					}
				}
			}
		}

		//работаем с камерой
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		//


		




		window.display();
	}

	return 0;
}

