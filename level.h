#pragma once
/*
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cctype>
#include <SFML/Graphics.hpp>
#include "tinyxml2.h"

//TinyXml2 / tinyxml2.h

class Object : public sf::FloatRect
{
public:

	Object(const std::string& name, const std::string& type, const sf::FloatRect& rect) :
		name_(name), type_(type), sf::FloatRect(rect) {}

	auto GetPropertyInt(const std::string& name) { return atoi(properties_[name].c_str()); }
	auto GetPropertyFloat(const std::string& name) { return strtof(properties_[name].c_str(), NULL); }
	auto& GetPropertyString(const std::string& name) { return properties_[name]; }

private:

	std::string                        name_;
	std::string                        type_;
	std::map<std::string, std::string> properties_;

	friend class TileMap;
};

class Layer {
public:

	sf::Uint8               opacity;
	std::vector<sf::Sprite> tiles;
};

class TileMap
{
public:

	TileMap() : width_(0u), height_(0u), tile_width_(0u), tile_height_(0u) {}

	~TileMap()
	{
		objects_.clear();
		layers_.clear();
	}

	bool LoadFromFile(const std::string& tmx_file_path, const std::string& tileset_path)
	{
		tinyxml2::XMLDocument levelFile;

		if (levelFile.LoadFile(tmx_file_path.c_str()) != tinyxml2::XML_SUCCESS) {
			std::cout << "Loading level \"" << tmx_file_path << "\" failed." << std::endl;
			return false;
		}



		tinyxml2::XMLElement* map(levelFile.FirstChildElement("map"));

		width_ = atoi(map->Attribute("width"));
		height_ = atoi(map->Attribute("height"));
		tile_width_ = atoi(map->Attribute("tilewidth"));
		tile_height_ = atoi(map->Attribute("tileheight"));

		if (!tileset_.loadFromFile(tileset_path)) {
			std::cout << "Loading level \"" << tileset_path << "\" failed." << std::endl;
			return false;
		}
		tileset_.setSmooth(true);

		size_t rows = tileset_.getSize().x / tile_width_;
		size_t columns = tileset_.getSize().y / tile_height_;

		//                                 Layers

		tinyxml2::XMLElement* layerElement(map->FirstChildElement("layer"));

		while (layerElement)
		{
			Layer layer;
			layer.tiles.reserve(width_ * height_);

			if (layerElement->Attribute("opacity"))
			{
				sf::Uint8 opacity = strtod(layerElement->Attribute("opacity"), NULL);
				layer.opacity = sf::Uint8(255 * opacity);
			}
			else {
				layer.opacity = (sf::Uint8)255;
			}

			tinyxml2::XMLElement* layerDataElement(layerElement->FirstChildElement("data"));

			if (!layerDataElement) {
				std::cout << "Bad map. No layer information found." << std::endl;
			}

			std::vector<sf::IntRect> rects;
			rects.reserve(rows * columns);

			for (int y = 0; y < columns; y++)
			{
				for (int x = 0; x < rows; x++)
				{
					rects.emplace_back(sf::IntRect(x * tile_width_,    // sf::IntRect::left
						y * tile_height_,   // sf::IntRect::top
						tile_width_,    // sf::IntRect::width
						tile_height_)); // sf::IntRect::height
				}
			}

			//                              Get parsed values
			std::vector<int> tilemap(std::move(parseCVScells(layerDataElement)));

			size_t cell(0u), n(0u);

			for (int y = 0; y < height_; y++)
			{
				for (int x = 0; x < width_; x++)
				{
					n = tilemap[cell];

					if (n > 0)
					{
						sf::Sprite sprite(tileset_, rects[n - 1]);
						sprite.setPosition(x * tile_width_, y * tile_height_);
						sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

						layer.tiles.emplace_back(sprite);
					}
					cell++;
				}
			}

			layers_.push_back(layer);
			layerElement = layerElement->NextSiblingElement("layer");
		}

		//                               Objects

		tinyxml2::XMLElement* objectGroupElement;

		if (map->FirstChildElement("objectgroup"))
		{
			objectGroupElement = map->FirstChildElement("objectgroup");

			while (objectGroupElement)
			{
				tinyxml2::XMLElement* objectElement(objectGroupElement->FirstChildElement("object"));

				while (objectElement)
				{
					std::string object_type;

					if (objectElement->Attribute("type")) {
						object_type = objectElement->Attribute("type");
					}

					std::string object_name;

					if (objectElement->Attribute("name")) {
						object_name = objectElement->Attribute("name");
					}

					auto x = static_cast<float>(atoi(objectElement->Attribute("x")));
					auto y = static_cast<float>(atoi(objectElement->Attribute("y")));
					auto width = static_cast<float>(atoi(objectElement->Attribute("width")));
					auto height = static_cast<float>(atoi(objectElement->Attribute("height")));

					Object object(object_name, object_type, sf::FloatRect(x, y, width, height));

					tinyxml2::XMLElement* properties(objectElement->FirstChildElement("properties"));

					if (properties)
					{
						tinyxml2::XMLElement* property(properties->FirstChildElement("property"));

						while (property)
						{
							object.properties_.emplace(property->Attribute("name"), property->Attribute("value"));
							property = property->NextSiblingElement("property");
						}

					}
					objects_.emplace_back(std::move(object));
					objectElement = objectElement->NextSiblingElement("object");
				}
				objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
			}
		}
		else
		{
			std::cout << "No object layers found..." << std::endl;
		}

		return true;
	}

	auto& GetObject(const std::string& name)
	{
		auto found = std::find_if(objects_.begin(), objects_.end(), [name](const Object& obj)
			{
				return obj.name_ == name;
			});

		return *found;
	}

	auto GetObjects(const std::string& name)
	{
		std::vector<Object> vec;

		std::copy_if(objects_.begin(), objects_.end(), std::back_inserter(vec), [name](const Object& obj) {
			return obj.name_ == name;
			});

		return vec;
	}

	std::vector<Object> GetAllObjects() {
		return objects_;
	};

	sf::Vector2i GetTileSize() {
		return sf::Vector2i(tile_width_, tile_height_);
	}

	// Convert CVS format to vector<int>, where each integer definite type of tile
	std::vector<int> parseCVScells(tinyxml2::XMLElement* element)
	{
		std::string dirty_string = element->GetText(), buff;
		size_t size = dirty_string.size();
		std::vector<int> vec;
		vec.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			if (isdigit(dirty_string[i])) {
				buff += dirty_string[i];
			}
			else if (!buff.empty()) {
				vec.emplace_back(std::stoi(buff));
				buff.clear();
			}
		}

		return vec;
	}

	void draw(sf::RenderTarget& target) const
	{
		const sf::FloatRect viewportRect = target.getView().getViewport();

		for (const auto& layer : layers_)
		{
			for (const auto& tile : layer.tiles)
			{
				if (viewportRect.intersects(tile.getLocalBounds())) {
					target.draw(tile);
				}
			}
		}
	}

private:

	size_t width_, height_, tile_width_, tile_height_;
	sf::Texture         tileset_;
	std::vector<Object> objects_;
	std::vector<Layer>  layers_;
};



*/

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TinyXML/tinyxml.h"

struct Object
{
	int GetPropertyInt(std::string name);//номер свойства объекта в нашем списке
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;//объявили переменную name типа string
	std::string type;//а здесь переменную type типа string
	sf::Rect<float> rect;//тип Rect с нецелыми значениями
	std::map<std::string, std::string> properties;//создаём ассоциатиный массив. ключ - строковый тип, значение - строковый

	sf::Sprite sprite;//объявили спрайт
};

struct Layer//слои
{
	int opacity;//непрозрачность слоя
	std::vector<sf::Sprite> tiles;//закидываем в вектор тайлы
};

class Level//главный класс - уровень
{
public:
	bool LoadFromFile(std::string filename);//возвращает false если не получилось загрузить
	Object GetObject(std::string name);
	std::vector<Object> GetObjects(std::string name);//выдаем объект в наш уровень
	std::vector<Object> GetAllObjects();//выдаем все объекты в наш уровень
	void Draw(sf::RenderWindow& window);//рисуем в окно
	sf::Vector2i GetTileSize();//получаем размер тайла

private:
	int width, height, tileWidth, tileHeight;//в tmx файле width height в начале,затем размер тайла
	int firstTileID;//получаем айди первого тайла
	sf::Rect<float> drawingBounds;//размер части карты которую рисуем
	sf::Texture tilesetImage;//текстура карты
	std::vector<Object> objects;//массив типа Объекты, который мы создали
	std::vector<Layer> layers;
};

///////////////////////////////////////


int Object::GetPropertyInt(std::string name)//возвращаем номер свойства в нашем списке
{
	return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
	return strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string name)//получить имя в виде строки.вроде понятно
{
	return properties[name];
}

bool Level::LoadFromFile(std::string filename)//двоеточия-обращение к методам класса вне класса 
{
	TiXmlDocument levelFile(filename.c_str());//загружаем файл в TiXmlDocument

	// загружаем XML-карту
	if (!levelFile.LoadFile())//если не удалось загрузить карту
	{
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;//выдаем ошибку
		return false;
	}

	// работаем с контейнером map
	TiXmlElement* map;
	map = levelFile.FirstChildElement("map");

	// пример карты: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	width = atoi(map->Attribute("width"));//извлекаем из нашей карты ее свойства
	height = atoi(map->Attribute("height"));//те свойства, которые задавали при работе в 
	tileWidth = atoi(map->Attribute("tilewidth"));//тайлмап редакторе
	tileHeight = atoi(map->Attribute("tileheight"));

	// Берем описание тайлсета и идентификатор первого тайла
	TiXmlElement* tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - путь до картинки в контейнере image
	TiXmlElement* image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// пытаемся загрузить тайлсет
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;//если не удалось загрузить тайлсет-выводим ошибку в консоль
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);//сглаживание

	// получаем количество столбцов и строк тайлсета
	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	// вектор из прямоугольников изображений (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}

	// работа со слоями
	TiXmlElement* layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;

		// если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = 255 * opacity;
		}
		else
		{
			layer.opacity = 255;
		}

		//  контейнер <data> 
		TiXmlElement* layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  контейнер <tile> - описание тайлов каждого слоя
		TiXmlElement* tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// Устанавливаем TextureRect каждого тайла
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(x * tileWidth, y * tileHeight);
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

				layer.tiles.push_back(sprite);//закидываем в слой спрайты тайлов
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// работа с объектами
	TiXmlElement* objectGroupElement;

	// если есть слои объектов
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			//  контейнер <object>
			TiXmlElement* objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// получаем все данные - тип, имя, позиция, и тд
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				// экземпляр объекта
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;

				// "переменные" объекта
				TiXmlElement* properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement* prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}

Object Level::GetObject(std::string name)
{
	// только первый объект с заданным именем
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name)
{
	// все объекты с заданным именем
	std::vector<Object> vec;
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);

	return vec;
}


std::vector<Object> Level::GetAllObjects()
{
	return objects;
};


sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow& window)
{
	// рисуем все тайлы (объекты не рисуем!)
	for (int layer = 0; layer < layers.size(); layer++)
		for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
			window.draw(layers[layer].tiles[tile]);
}








