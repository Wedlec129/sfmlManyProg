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
	vector<Object> obj;//������ �������� ����� � ������� ����� �����������������

	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health;
	bool life, onGround;
	Texture t;
	Sprite s;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false;
		t.loadFromImage(image);
		s.setTexture(t);
		s.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}



	

};





class Player :public Entity {
public:

	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������

	int playerScore;


	Player(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lvl.GetAllObjects();//��������������.�������� ��� ������� ��� �������������� ��������� � ������
		if (name == "Player1") {
			s.setTextureRect(IntRect(4, 19, w, h));
		}



	}








	void control() {
		if (Keyboard::isKeyPressed) {//���� ������ �������
			if (Keyboard::isKeyPressed(Keyboard::A)) {//� ������ �����
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right; speed = 0.1;
			}

			if (((Keyboard::isKeyPressed(Keyboard::W)) || (Keyboard::isKeyPressed(Keyboard::Space))) && (onGround)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
				state = jump; dy = -0.6; onGround = false;//�������� ������ ������
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{

		for (int i = 0; i < obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }                   /////����/���� ���� ���� ������� - ������ ������ ������
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }               /////�����//���� ���� ���� ������� + ������ ���� �������
					if (Dx > 0) { x = obj[i].rect.left - w; }                                      /////� �����///���� ����� ���� ���� ������� - ������ ������
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }                   /////� ����//���� ����� ���� ���� ������� + ���������� �������
				} 
			}


		

	}

	void update(float time) 
	{
		control();//������� ���������� ����������
		switch (state)//��� �������� ��������� �������� � ����������� �� ���������
		{
		case right:dx = speed; break;//��������� ���� ������
		case left:dx = -speed; break;//��������� ���� �����
		case up: break;//����� ��������� �������� ������ (�������� �� ��������)
		case down: dx = 0; break;//����� ��������� �� ����� ������ ��������� (�������� �� ��������)
		case stay: break;//� ����� ����		
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		y += dy * time;
		checkCollisionWithMap(0, dy);//������������ ������������ �� Y
		s.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������

		speed = 0; //����� ���� �� ��� ����������

		if (health <= 0) { life = false; }



		dy = dy + 0.0015 * time;//��������� ������������� � �����
	}

	float getplayercoordinateX() {	//���� ������� ����� �������� ���������� �	
		return x;
	}

	float getplayercoordinateY() {	//���� ������� ����� �������� ���������� Y 	
		return y;
	}


	void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������


		view.setCenter(x + 100, y); //������ �� �������, ��������� ��� ���������� ������. +100 - �������� ������ �� ���� ������. �����������������


	}






};

class Enemy :public Entity {
public:
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//��������������.�������� ������ ������� ��� �������������� ����� � ������
		if (name == "EasyEnemy") {
			s.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;//���� ��������.���� ������ ������ ���������
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{

		for (int i = 0; i < obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					//if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				//	if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; s.scale(-1, 1); } //���� ���� �� ��� � ����� �� ,, ����� ���� ���� ������� � ������� ����������� - ������ ������ ����� ,, DX ������������� � �������� ����� �� x
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; s.scale(-1, 1); }
				}
			}



	}

	void update(float time) 
	{
		if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����


			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			x += dx * time;
			s.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(1366, 768), "lol");
	view.reset(FloatRect(0, 0, 840, 580));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������
	setlocale(LC_ALL, "Russian");



	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Image enimyImages;
	enimyImages.loadFromFile("images/shamaich.png");


	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.

	Object player = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	//Object easyEnemyObject = lvl.GetObject("easyEnemy");//������ ������� ����� �� ����� �����.������ ���������� ������ � ������ ��� ������ ����





	list<Enemy*>  easyEnemy;//������ ������, ���� ���� ������ �������.�������� ������.
	
	vector<Object> e = lvl.GetObjects("EasyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������

	for (int i = 0; i < e.size(); i++)//���������� �� ��������� ����� �������(� ������ �� ������)
		easyEnemy.push_back(new Enemy(enimyImages, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));//� ���������� � ������ ���� ����� ������ � ���





	
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	//Enemy e(enimyImages, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);//�������� ���������� �������������� easyEnemy �� ����� � ���������� ������ �����



	float ani = 0;
	Clock clock;


	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;



		Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� ������� c ������ �������� ����
		Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (Keyboard::isKeyPressed(Keyboard::Escape)))
				window.close();

		}
		window.clear();






		/////////////////////////////������ �����/////////////////////
		
		lvl.Draw(window);

		/////////////
	
		//��������� � ������ �������� �����
		p.update(time);
		window.draw(p.s);


		//��������� � ������ �����
		for (auto it = easyEnemy.begin(); it != easyEnemy.end(); it++) { 
			
			(*it)->update(time); 
			window.draw((*it)->s); //������ entities ������� (������ ��� ������ �����)


		}




		//�������� � �������
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		//


		




		window.display();
	}

	return 0;
}
