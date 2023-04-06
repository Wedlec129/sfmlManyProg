#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <math.h>
#include "map.h"
#include "view.h"
#include "mission.h"
using namespace std;
using namespace sf;


class Player { 
private:
	//float x, y, dx , w ,h , dy=0;

public:

	float x, y, dx, w, h, dy = 0;

	bool isMove;
	bool isSelect;//�������� ���������� ��������� �������� � ������ �������

	
	float speed = 0; 
	int dir = 0;
	

	int ball=0;

	int health =1000;
	bool life=true;
	

	Texture t;
	Sprite s;

	Player (String Faile, float X, float Y, float W, float H) {  
		
		isMove = false;
		isSelect = false;
		
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

	float getplayercoordinateX() {	//���� ������� ����� �������� ���������� �	
		return x;
	}

	float getplayercoordinateY() {	//���� ������� ����� �������� ���������� Y 	
		return y;
	}


	void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������


		view.setCenter(x + 100, y); //������ �� �������, ��������� ��� ���������� ������. +100 - �������� ������ �� ���� ������. �����������������


	}



	void interactionWithMap()//�-��� �������������� � ������
	{



		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j < (x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (dy > 0)//���� �� ��� ����,
					{
						y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//���������� ���� �����
					}
				}

				if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
					ball++;
					TileMap[i][j] = ' ';//������� ������, ���� ����� �����. ����� � �� �������, ������.
				}

				if (TileMap[i][j] == 'f') {
					health = health- 40;//���� ����� ����������� � ���� ������,�� ���������� health=health-40;
					TileMap[i][j] = ' ';//������ ������
				}

				if (TileMap[i][j] == 'h') {
					health = health+ 20;//���� ����� ��������,�� ���������� health=health+20;
					TileMap[i][j] = ' ';//������ ��������
				}


			}
	}



};



int main()
{
	RenderWindow window(VideoMode(1366, 768), "lol");
	view.reset(FloatRect(0, 0, 840, 580));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������
	setlocale(LC_ALL, "Russian");
	
	Player p("images/hero.png", 250, 250, 96, 96);


	float ani = 0;
	Clock clock;
	
	
	Texture mapT;
	mapT.loadFromFile("images/map.png");
	Sprite mapS;
	mapS.setTexture(mapT);



	//

	bool showMissionText = true;//���������� ����������, ���������� �� ��������� ������ ������ �� ������

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.png");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
	s_quest.setScale(0.6f, 0.6f);//���� ��������� ��������, => ������ ���� ������
	

	Font fontTextMision;
	fontTextMision.loadFromFile("font/bahnschrift.ttf");

	Text textMision;
	textMision.setFont(fontTextMision);

	Font fontBall;
	fontBall.loadFromFile("font/bahnschrift.ttf");

	Text textBall;
	textBall.setFont(fontBall);


	
		int createObjectForMapTimer = 0;




		int tempX = 0;//��������� ����� �.������� �� ����� ������� ���� ���� ����
		int tempY = 0;//����� Y
		float distance = 0;//��� ���������� �� ������� �� ���� �������


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

			if ( event.type == Event::KeyPressed)//������� ������� �������
				if ((event.key.code == Keyboard::Tab))  {//���� ������� ���


					switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText

					case true: {
						

						string task;//������ ������ ������
						task = getTextMission(getNomerMission(p.getplayercoordinateX()));//���������� ������� getTextMission (��� ���������� ����� ������), ������� ��������� � �������� ��������� ������� getCurrentMission(������������ ����� ������), � ��� ��� �-��� ��������� � �������� ��������� ������� p.getplayercoordinateX() (��� �-��� ���������� ��� ���������� ������)

						textMision.setString(task);//������
						textMision.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����

						s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����

						//ball
						ostringstream playerBall;
						playerBall << p.ball;
						textBall.setString("ball: " + playerBall.str());
						textBall.setFillColor(Color(255, 10, 29));
						textBall.setPosition(view.getCenter().x + 125, view.getCenter().y - 160);

						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
					}
					case false: {
						
						showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
						break;
					};
					};




				}


			if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
				if (event.key.code == Mouse::Left) {//� ������ �����
					if (p.s.getGlobalBounds().contains(pos.x, pos.y))//� ��� ���� ���������� ������� �������� � ������
					{
						p.s.setColor(Color::Green);//������ ������ � �������,��� ����� ������ ������,��� �� ������ ��������� � ����� ������� ���
						p.isSelect = true;
					}

				};
			

		}
		window.clear();

		
		if (p.isSelect)//���� ������� ������
			if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
				if (event.key.code == Mouse::Right) {//� ������ ������
					p.isMove = true;//�� �������� ��������
					p.isSelect = false;//������ ��� �� ������
					p.s.setColor(Color::White);//���������� ������� ���� �������
					tempX = pos.x;//�������� ���������� ������� ������� �
					tempY = pos.y;//� Y

				}



		if (p.isMove) {
			distance = sqrt((tempX - p.x) * (tempX - p.x) + (tempY - p.y) * (tempY - p.y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������

			if (distance > 2) {//���� �������� ������� �������� �� ����� �������� ������� �������

				p.x += 0.1 * time * (tempX - p.x) / distance;//���� �� ���� � ������� ������� �������
				p.y += 0.1 * time * (tempY - p.y) / distance;//���� �� ������ ��� ��
			}
			else { p.isMove = false; std::cout << "priehali\n"; }//������� ��� ��� ������ �� ���� � ������� ������� ��������� � �������
		}



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
			p.interactionWithMap();
			//
		if(createObjectForMapTimer<=3000)
			createObjectForMapTimer += time;//���������� ������

			if (createObjectForMapTimer == 3000) {
				randomMapGenerate();//��������� ���� ������
				createObjectForMapTimer = 0;//�������� ������
			}

		//
		//

		p.update(time);
		window.draw(p.s);
		window.setView(view);
		p.getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());

		//����������
		float jostikX =Joystick::getAxisPosition(0, Joystick::X);
		float jostikY(Joystick::getAxisPosition(0, Joystick::Y));

		if (p.life==true && showMissionText==true && p.isMove==false) {



			if (Keyboard::isKeyPressed(Keyboard::D)|| jostikX>70) {

				


				p.dir = 0; p.speed = 0.1;

				ani = ani + 0.005 * time;
				if (ani > 3) ani = 0;
				p.s.setTextureRect(IntRect(96 * int(ani), 192, 96, 96));

				if (Keyboard::isKeyPressed(Keyboard::LShift) || Joystick::isButtonPressed(0,1)) {

					p.speed = 0.9;

				}



			};

			if (Keyboard::isKeyPressed(Keyboard::A) || jostikX <-70) {


				p.dir = 1; p.speed = 0.1;

				ani = ani + 0.005 * time;
				if (ani > 3) ani = 0;
				p.s.setTextureRect(IntRect(96 * int(ani), 96, 96, 96));

				if (Keyboard::isKeyPressed(Keyboard::LShift) || Joystick::isButtonPressed(0, 1)) {

					p.speed = 0.9;

				}

			}

			if (Keyboard::isKeyPressed(Keyboard::W) || jostikY < -70) {

				

				p.dir = 3; p.speed = 0.1;

				ani = ani + 0.005 * time;
				if (ani > 3) ani = 0;
				p.s.setTextureRect(IntRect(96 * int(ani), 288, 96, 96));

				if (Keyboard::isKeyPressed(Keyboard::LShift) || Joystick::isButtonPressed(0, 1)) {

					p.speed = 0.9;

				}

			}

			if (Keyboard::isKeyPressed(Keyboard::S) || jostikY > 70) {

				
				p.dir = 2; p.speed = 0.1;

				ani = ani + 0.005 * time;
				if (ani > 3) ani = 0;
				p.s.setTextureRect(IntRect(96 * int(ani), 0, 96, 96));

				if (Keyboard::isKeyPressed(Keyboard::LShift) || Joystick::isButtonPressed(0, 1)) {

					p.speed = 0.9;

				}

			}

		};
		


		if (!showMissionText) {
			textMision.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����			
			window.draw(s_quest);
			window.draw(textMision); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
			window.draw(textBall);
		}



		window.display();
	}

	return 0;
}