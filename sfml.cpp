
//cd Desktop; g++ main.cpp -o sfml-app -L /Users/wedlec/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system;./sfml-app;
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <math.h>
#include <list>
#include <vector>
using namespace std;
using namespace sf;

//болте удобное создание sprite
class WeSprite {
private:

	Image i;
	Texture t;
	Sprite s;

	int x, y, w, h;//



public:
	 ////не сов
	WeSprite(int X, int Y, int W, int H) {

		i.loadFromFile("/Users/wedlec/Desktop/картинки/собакен-кусака/dog.png");
		i.createMaskFromColor(Color(255, 255, 255));
		t.loadFromImage(i);

		x = X; y = Y; w = W; h = H;

		s.setTextureRect(IntRect(x, y, w, h));

		s.setTexture(t);
	}

 
    void setPosition(int X, int Y){

    s.setPosition(X,Y);
    

}
    

	void draw(RenderWindow& window) {


		window.draw(s);

	}


};

//текст
class WeText {
private:

	string shribe;


	Font font;

public:



	Text txt;

	//несов
	WeText(string shribeName) {


		font.loadFromFile("/Library/Fonts/Arial Unicode.ttf");

		txt.setFont(font);
		txt.setCharacterSize(20);

		txt.setOrigin(txt.getScale().x / 2, txt.getScale().y / 2);


		//начальный цвет
		txt.setFillColor(Color(0, 0, 0));

		txt.setString(shribeName);



		shribe = shribeName;

	}


	//текст будет следить за спрайтом
	virtual void sleditForSprite(Sprite& s, float x, float y) {

		txt.setPosition(s.getPosition().x + x, s.getPosition().y + y);



	}

	//дать число тексту
	void getChislo(float n) {



		ostringstream chislo;

		chislo << n;

		txt.setString(shribe + chislo.str());





	}




	//ф-€ дл€ текста
	void setString(string name) {

		txt.setString(name);


	}

	virtual void setPosition(float x, float y) {

		txt.setPosition(x, y);

	}

	void setFillTextColor(float R, float G1, float B) {

		txt.setFillColor(Color(R, G1, B));

	}
	//

	//помен€ть размер шрифта
	void setCharacterSize(float a) {


		txt.setCharacterSize(a);

	}


	virtual void setRotate(int rot) {

		txt.setRotation(rot);
		

	}
	int getRotate() {

		return txt.getRotation();

	}

	//нарисовать в окне
	virtual void draw(RenderWindow& window) {


		window.draw(txt);



	};


};




int main()
{
    RenderWindow window(sf::VideoMode(900, 900), "SFML works!");
    CircleShape shape(100.f);
    shape.setFillColor(Color(Color(255,0,0)));


    WeText txt("lol kek");
    txt.setFillTextColor(0,255,0);
    txt.setPosition(250,100);
    txt.setCharacterSize(70);

    WeSprite sprite(0,0,410,410);
    sprite.setPosition(100,250);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();



        window.draw(shape);
        txt.draw(window);
        sprite.draw(window);




        window.display();
    }

  
    
    
    return 0;
}
