/*
* @author PELLETIER Benoit
*
* @file Utils.cpp
*
* @date 04/10/2018
*
* @brief Some utility functions
*
*/

#include "stdafx.h"
#include "Utils.h"

string readFile(string filename)
{
	string text = "";
	int length;
	char buffer[MAX_BUFFER_SIZE] = { 0 };

	ifstream file;
	file.open(filename);

	if (file.is_open())
	{
		file.seekg(0, ios_base::end);
		length = (int)file.tellg();
		file.seekg(0, ios_base::beg);
		if (length > MAX_BUFFER_SIZE)
		{
			length = MAX_BUFFER_SIZE;
		}

		file.read(buffer, length);

		file.close();

		text = buffer;
	}
	else
	{
		printf("Error when loading file %s\n", filename.c_str());
	}


	return text;
}

string floatToStr(float number, int precision)
{
	stringstream ss;
	ss << fixed << setprecision(precision) << number;
	return ss.str();
}

void drawPixel(sf::RenderWindow &window, sf::Vector2f pixel, sf::Color color)
{
	sf::Vertex v(pixel, color);
	window.draw(&v, 1, sf::PrimitiveType::Points);
}

void drawLine(sf::RenderWindow &window, sf::Vector2f p1, sf::Vector2f p2, sf::Color color)
{
	sf::Vertex v[2] = { sf::Vertex(p1, color), sf::Vertex(p2, color) };
	window.draw(v, 2, sf::PrimitiveType::Lines);
}

float mapValue(float x, float p00, float p01, float p10, float p11)
{
	float newValue = 0.0f;
	if (p00 != p01 && p10 != p11)
	{
		float a = x - p00;
		//float b ? ;
		float max0 = p01 - p00;
		float max1 = p11 - p10;
		newValue = p10 + a * (max1 / max0);
	}

	return newValue;
}
