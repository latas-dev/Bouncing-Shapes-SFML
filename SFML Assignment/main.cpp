#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

static sf::Font myFont;
static int wWidth;
static int wHeight;

static std::tuple<float, float> getShapeTextPos(sf::FloatRect shapeBounds, sf::FloatRect textBounds)
{
	float textPosX = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2;
	float textPosY = shapeBounds.top + (shapeBounds.height / 2) - textBounds.height;
	return std::make_tuple(textPosX, textPosY);
}

class Circle
{
public:

	std::string		m_name;
	sf::Text		m_text;
	sf::CircleShape	m_circle;
	float			m_xs;
	float			m_ys;

	Circle(std::string name, int x, int y, float xs, float ys, int r, int g, int b, int radius, int fontSize, sf::Color fontColor)
		: m_name(name)
		, m_circle(radius)
		, m_xs(xs)
		, m_ys(ys)
	{
		m_circle.setFillColor(sf::Color(r, g, b));
		m_circle.setPosition((float)x, (float)y);

		m_text.setFont(myFont);
		m_text.setString(m_name);
		m_text.setCharacterSize(fontSize);
		m_text.setFillColor(fontColor);
		std::tuple<float, float> textBounds = getShapeTextPos(m_circle.getGlobalBounds(), m_text.getGlobalBounds());
		m_text.setPosition(std::get<0>(textBounds), std::get<1>(textBounds));
	}

	void moveShape()
	{
		sf::Vector2f shapePos = m_circle.getPosition(), shapeTextPos = m_text.getPosition();

		sf::FloatRect bounds = m_circle.getGlobalBounds();

		if (bounds.top <= 0)
		{
			m_ys = std::abs(m_ys);
		}
		else if ((bounds.top + bounds.height) >= wHeight)
		{
			m_ys = -std::abs(m_xs);
		}
		else if (bounds.left <= 0)
		{
			m_xs = std::abs(m_xs);
		}
		else if ((bounds.left + bounds.width) >= wWidth)
		{
			m_xs = -std::abs(m_xs);
		}

		shapePos.x += m_xs;
		shapePos.y += m_ys;
		shapeTextPos.x += m_xs;
		shapeTextPos.y += m_ys;

		m_circle.setPosition(shapePos);
		m_text.setPosition(shapeTextPos);
	}
};

class Rectangle
{
public:

	std::string			m_name;
	sf::Text			m_text;
	sf::RectangleShape	m_rect;
	float				m_xs;
	float				m_ys;

	Rectangle(std::string name, int x, int y, float xs, float ys, int r, int g, int b, sf::Vector2f vector,  int fontSize, sf::Color fontColor)
		: m_name(name)
		, m_rect(vector)
		, m_xs(xs)
		, m_ys(ys)
	{
		m_rect.setFillColor(sf::Color(r, g, b));
		m_rect.setPosition((float) x, (float) y);

		m_text.setFont(myFont);
		m_text.setString(m_name);
		m_text.setCharacterSize(fontSize);
		m_text.setFillColor(fontColor);
		std::tuple<float, float> textBounds = getShapeTextPos(m_rect.getGlobalBounds(), m_text.getGlobalBounds());
		m_text.setPosition(std::get<0>(textBounds), std::get<1>(textBounds));
	}

	void moveShape()
	{
		sf::Vector2f shapePos = m_rect.getPosition(), shapeTextPos = m_text.getPosition();

		sf::FloatRect bounds = m_rect.getGlobalBounds();

		if (bounds.top <= 0)
		{
			m_ys = std::abs(m_ys);
		}
		else if ((bounds.top + bounds.height) >= wHeight)
		{
			m_ys = -std::abs(m_xs);
		}
		else if (bounds.left <= 0)
		{
			m_xs = std::abs(m_xs);
		}
		else if ((bounds.left + bounds.width) >= wWidth)
		{
			m_xs = -std::abs(m_xs);
		}

		shapePos.x += m_xs;
		shapePos.y += m_ys;
		shapeTextPos.x += m_xs;
		shapeTextPos.y += m_ys;

		m_rect.setPosition(shapePos);
		m_text.setPosition(shapeTextPos);
	}
};

int main(int argc, char** argv)
{
	std::ifstream input("./config.txt");

	if (!input.is_open())
	{
		std::cerr << "Failed to open config file" << std::endl;
		return 1;
	}

	std::string item;
	sf::RenderWindow window;
	int fontSize = 12;
	sf::Color fontColor;

	if (input >> item)
	{
		input >> wWidth >> wHeight;

		window.create(sf::VideoMode(wWidth, wHeight), "Assignment 1");
	}
	if (input >> item)
	{
		std::string path;
		int size, r, g, b;

		input >> path >> size >> r >> g >> b;
		if (!myFont.loadFromFile(path))
		{
			std::cerr << "Couldn't find Font" << std::endl;
			return 1;
		}
		fontSize = size;
		fontColor = sf::Color(r, g, b);
	}

	std::vector<Rectangle> rectangleVector;
	std::vector<Circle> circleVector;

	while (input >> item)
	{
		std::string name;
		int x, y, r, g, b;
		float xs, ys;

		input >> name >> x >> y >> xs >> ys >> r >> g >> b;

		if (item == "Rectangle")
		{
			int w, h;
			input >> w >> h;

			// create rectangle shape
			// add rectangle shape to vector
			rectangleVector.push_back(Rectangle(name, x, y, xs, ys, r, g, b, sf::Vector2f(w, h), fontSize, fontColor));

		} else if (item == "Circle")
		{
			int radius;
			input >> radius;

			// create circle shape
			// add circle shape to vector
			circleVector.push_back(Circle(name, x, y, xs, ys, r, g, b, radius, fontSize, fontColor));
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) 
				window.close();
		}

		window.clear();
		//
		for (size_t i = 0; i < rectangleVector.size(); ++i)
		{
			window.draw(rectangleVector[i].m_rect);
			window.draw(rectangleVector[i].m_text);
			rectangleVector[i].moveShape();
		}
		for (size_t i = 0; i < circleVector.size(); ++i)
		{
			window.draw(circleVector[i].m_circle);
			window.draw(circleVector[i].m_text);
			circleVector[i].moveShape();
		}
		//
		window.display();
	}

	return 0;
}