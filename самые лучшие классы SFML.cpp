//болте удобное создание sprite
class WeSprite {
private:

	Image i;
	Texture t;
	Sprite s;

	int x, y, w, h;



public:
	 ////не сов
	WeSprite(int X, int Y, int W, int H) {

		i.loadFromFile("image/pidor.png");
		i.createMaskFromColor(Color(255, 255, 255));
		t.loadFromImage(i);

		x = X; y = Y; w = W; h = H;

		s.setTextureRect(IntRect(x, y, w, h));

		s.setTexture(t);
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


		font.loadFromFile("font/bahnschrift.ttf");

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




	//ф-я для текста
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

	//поменять размер шрифта
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

//кнопка
class WeButton :public WeText {
private:

	float smechTextX, smechTextY; //типо на сколько смещать текст по x и y



	float w, h; //ширена и высота кнопки

	SoundBuffer buffer;

	Sound sound;   //звук

	bool press;   // 


public:





	RectangleShape button;//сам крвадрат



	
	WeButton(float W, float H, string shribeName) :WeText(shribeName) {

		smechTextX = 0; smechTextY=0;
		w = W;
		h = H;

		press = false;

		//button.setOrigin(w / 2, h / 2);

		button.setSize(Vector2f(W, H));

		txt.setPosition(button.getPosition());




	}

	//кнопка следит за спрайтом
	void sleditForSprite(Sprite& s, float x, float y) override {


		button.setPosition(s.getPosition().x + x, s.getPosition().y + y);
		txt.setPosition(s.getPosition().x + x, s.getPosition().y + y);



	}

	void setPositonText(float x2, float y2) {

		smechTextX = x2;
		smechTextY = y2;

	}



	//рисуеи и кнопк(квадрат) и текст
	void draw(RenderWindow& window)override {

		txt.setPosition(button.getPosition().x+ smechTextX, button.getPosition().y+smechTextY);


		window.draw(button);
		window.draw(txt);
	}








	//нажатие
	bool pressed(Event& event, Vector2i  pos) {





		if (button.getGlobalBounds().contains(pos.x, pos.y) && event.type == Event::MouseButtonPressed && !press) {



			if (event.key.code == Mouse::Left) {


				press = true;
				return true;
			}





		};







		if (!(press)) return false;


		if (press) {

			if (!(event.type == Event::MouseButtonPressed)) {

				press = false;




			}

			return false;



		}




	};

	//наведина мышка
	bool navediaMouse(Event& event, Vector2i  pos) {


		if (button.getGlobalBounds().contains(pos.x, pos.y)) return true;

		else return false;



	}




	//дать звук
	void getSound(string failAudio) {

		buffer.loadFromFile(failAudio);

		sound.setBuffer(buffer);




	}


	//играть звук
	void soundPlay() {

		sound.play();

	}

	//поменять громкость
	void soundSetVolume(int volume) {

		sound.setVolume(volume);

	}






	//поменять размеры квадрата кнопки 
	void setButtonSize(float W, float H) {

		w = W;
		h = H;

		button.setSize(Vector2f(W, H));


	}




		
	void setRotate(int rot) {

		button.setRotation(rot);
		txt.setRotation(rot);

	}




	//поменять позицию
	void setPosition(float x, float y)  override {

		button.setPosition(x, y);

	}

	//поменять цвет квадрата
	void setFillRacktengelColor(float R, float G1, float B) {

		button.setFillColor(Color(R, G1, B));



	}





};

//кнопка которая как картина
class WeButtonImage: public WeText{
private:

	float smechTextX, smechTextY;  //типо на сколько смещать текст по x и y


	float w, h; //ширена и высота кнопки

	SoundBuffer buffer;

	Sound sound;   //звук

	bool press;   // 

	Image i;
	Texture t;



public:





	Sprite button;//сам крвадрат



	
	WeButtonImage(float x,float y,float W, float H, string shribeName) :WeText(shribeName) {

		smechTextX = 0; smechTextY = 0;


		i.loadFromFile("image/pidor.png");
		i.createMaskFromColor(Color(255, 255, 255));
		t.loadFromImage(i);


		w = W;h = H;
		button.setTextureRect(IntRect(x, y, w, h));

		button.setTexture(t);

		press = false;

		//button.setOrigin(w / 2, h / 2);

		//button.setScale(Vector2f(W, H));

		txt.setPosition(button.getPosition());




	}

	//кнопка следит за спрайтом
	void sleditForSprite(Sprite& s, float x, float y) override {


		button.setPosition(s.getPosition().x + x, s.getPosition().y + y);
		txt.setPosition(s.getPosition().x + x, s.getPosition().y + y);



	}

	void setPositonText(float x2, float y2) {

		smechTextX = x2;
		smechTextY = y2;

	}

	//рисуеи и кнопк(квадрат) и текст
	void draw(RenderWindow& window)override {

		txt.setPosition(button.getPosition().x+ smechTextX, button.getPosition().y+smechTextY);


		window.draw(button);
		window.draw(txt);
	}








	//нажатие
	bool pressed(Event& event, Vector2i  pos) {





		if (button.getGlobalBounds().contains(pos.x, pos.y) && event.type == Event::MouseButtonPressed && !press) {



			if (event.key.code == Mouse::Left) {


				press = true;
				return true;
			}





		};







		if (!(press)) return false;


		if (press) {

			if (!(event.type == Event::MouseButtonPressed)) {

				press = false;




			}

			return false;



		}




	};

	//наведина мышка
	bool navediaMouse(Event& event, Vector2i  pos) {


		if (button.getGlobalBounds().contains(pos.x, pos.y)) return true;

		else return false;



	}




	//дать звук
	void getSound(string failAudio) {

		buffer.loadFromFile(failAudio);

		sound.setBuffer(buffer);




	}


	//играть звук
	void soundPlay() {

		sound.play();

	}

	//поменять громкость
	void soundSetVolume(int volume) {

		sound.setVolume(volume);

	}




	void setRotate(int rot) {

		button.setRotation(rot);
		txt.setRotation(rot);

	}




	//поменять позицию
	void setPosition(float x, float y)  override {

		button.setPosition(x, y);

	}

	//поменять цвет квадрата
	void setFillRacktengelColor(float R, float G1, float B) {

		button.setColor(Color(R, G1, B));



	}





};

//слайдер
class WeSlider {
private:

	//несов только гор!!! итд


	float WsliderRectandel, HsliderRectandel; //ширена и высота кнопки
	float Wslider, Hslider; //ширена и высота кнопки

	int dop; //типо когда мы движем слайдер  и типо мы доюовляем ширину его типо ведь у нас НН коар в нач 

	bool press;   // 

public:

	RectangleShape sliderRectandel;

	RectangleShape slider;


	RectangleShape sliderPoloska;


	


	WeSlider(float WsliderRectandel1,float HsliderRectandel1, float Wslider2,float Hslider2) {

		WsliderRectandel = WsliderRectandel1; HsliderRectandel = HsliderRectandel1;
		Wslider = Wslider2; Hslider = Hslider2;

		dop = 0;


		sliderRectandel.setOrigin(WsliderRectandel / 2, HsliderRectandel / 2);
		slider.setOrigin(Wslider / 2, Hslider / 2);


		
		sliderPoloska.setSize(Vector2f(WsliderRectandel, Hslider));
		sliderPoloska.setFillColor(Color::Black);


		sliderRectandel.setSize(Vector2f(WsliderRectandel, HsliderRectandel));

		slider.setSize(Vector2f(Wslider, Hslider));

		press = false;


	}


	void setPosition(float x, float y) {

		sliderRectandel.setPosition(x+ WsliderRectandel / 2, y+ HsliderRectandel / 2);

		slider.setPosition(x+ Wslider / 2, y + HsliderRectandel / 2);


	}

	void setColorRectangel(float R, float G, float B) {

		sliderRectandel.setFillColor(Color(R, G, B));


	}

	void setColorsliderPoloska(float R, float G, float B) {

		sliderPoloska.setFillColor(Color(R, G, B));


	}

	void setColorSlider(float R, float G, float B) {

		slider.setFillColor(Color(R, G, B));


	}

	float sliderVolume() {

		int a= ((sliderRectandel.getPosition().x - WsliderRectandel / 2 - slider.getPosition().x - Wslider / 2)*-1- Wslider+dop);


		return a;
	}




	//рисуеи и кнопк(квадрат) и текст
	void draw(RenderWindow& window, Event& event, Vector2i  pos) {

		


		if (slider.getGlobalBounds().contains(pos.x, pos.y) ) {



			if (Mouse::isButtonPressed(Mouse::Left)) {


                   slider.setPosition(pos.x, slider.getPosition().y);
				

					if (slider.getPosition().x + Wslider / 2 > sliderRectandel.getPosition().x + WsliderRectandel / 2) {


						//cout << "stopR"<<endl;


						slider.setPosition(sliderRectandel.getPosition().x + WsliderRectandel / 2 - Wslider / 2, slider.getPosition().y);

						
						dop = Wslider;
						

					}
					else {

						dop = 0;
					}

					
					

				

					if (slider.getPosition().x - Wslider / 2 < sliderRectandel.getPosition().x - WsliderRectandel / 2) {


						//cout << "stopL"<<endl;

						slider.setPosition(sliderRectandel.getPosition().x - WsliderRectandel / 2 + Wslider / 2, slider.getPosition().y);



					}


			}



		};



		window.draw(sliderRectandel);

		sliderPoloska.setPosition(sliderRectandel.getPosition().x- WsliderRectandel/2, sliderRectandel.getPosition().y- Hslider/2);
		window.draw(sliderPoloska);

		window.draw(slider);
	}




	//нажатие
	bool pressedSlider(Event& event, Vector2i  pos) {





		if (slider.getGlobalBounds().contains(pos.x, pos.y) && event.type == Event::MouseButtonPressed && !press) {



			if (event.key.code == Mouse::Left) {


				press = true;
				return true;
			}





		};







		if (!(press)) return false;


		if (press) {

			if (!(event.type == Event::MouseButtonPressed)) {

				press = false;




			}

			return false;



		}




	};

	//наведина мышка
	bool navediaMouseInSlider(Event& event, Vector2i  pos) {


		if (slider.getGlobalBounds().contains(pos.x, pos.y)) return true;

		else return false;



	}








};
