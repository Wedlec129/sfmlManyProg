#include <math.h>
#include "map.h"
#include "view.h"
#include "mission.h"
using namespace std;
using namespace sf;



class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health;
	bool life, isMove, onGround;
	Texture t;
	Sprite s;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image& image,String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		t.loadFromImage(image);
		s.setTexture(t);
		s.setOrigin(w / 2, h / 2);
	}
};





class Player :public Entity {
public:

    enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������

	int playerScore;


	Player(Image& image, String Name, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay;
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

			if (((Keyboard::isKeyPressed(Keyboard::W))|| (Keyboard::isKeyPressed(Keyboard::Space))) && (onGround)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
				state = jump; dy = -0.6; onGround = false;//�������� ������ ������
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����? ��
				{
					if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; }// � ����� ����� �����
				}
				//else { dx += 0.00015; }//���� ������ �.� �� ����� ���������� � �� ������ ����������� ��� ��������� ������� �������� ����
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

		speed = 0;

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



int main()
{
	RenderWindow window(VideoMode(1366, 768), "lol");
	view.reset(FloatRect(0, 0, 840, 580));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������
	setlocale(LC_ALL, "Russian");
	
	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");
	
	Player p(heroImage, "Player1", 750, 500, 40, 30);


	


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
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')  mapS.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
					if (TileMap[i][j] == 's')  mapS.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
					if ((TileMap[i][j] == '0')) mapS.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
					if ((TileMap[i][j] == 'f')) mapS.setTextureRect(IntRect(96, 0, 32, 32));//�������� ������
					if ((TileMap[i][j] == 'h')) mapS.setTextureRect(IntRect(128, 0, 32, 32));//� ��������

					mapS.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

					window.draw(mapS);//������ ���������� �� �����
				}

			//�������������� � ������ 
			
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