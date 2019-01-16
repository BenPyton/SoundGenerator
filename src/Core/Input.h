/*
* @author PELLETIER Benoit
*
* @file Input.h
*
* @date 04/10/2018
*
* @brief Manage SFML inputs
*
*/

#ifndef _INPUT_H
#define _INPUT_H

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

class Input
{
private:
	static Input m_instance;

	bool m_keyPressed[sf::Keyboard::Key::KeyCount];
	bool m_key[sf::Keyboard::Key::KeyCount];
	bool m_keyReleased[sf::Keyboard::Key::KeyCount];

	bool m_mousePressed[sf::Mouse::ButtonCount];
	bool m_mouse[sf::Mouse::ButtonCount];
	bool m_mouseReleased[sf::Mouse::ButtonCount];

	float m_mouseScroll;
	sf::Vector2f m_mousePos;

	sf::String m_enteredText;

	Input();
	void _Update(/*sf::RenderWindow &window*/);

public:
	~Input();

	static void Update(/*sf::RenderWindow &window*/);

	static bool GetKeyDown(sf::Keyboard::Key key);
	static bool GetKeyUp(sf::Keyboard::Key key);
	static bool GetKey(sf::Keyboard::Key key);

	static bool GetMouseButtonDown(sf::Mouse::Button  button);
	static bool GetMouseButtonUp(sf::Mouse::Button  button);
	static bool GetMouseButton(sf::Mouse::Button button);
	static float GetMouseWheelScroll();

	static sf::Vector2f GetMousePosition();

	static sf::String GetEnteredText();
};

#endif // _INPUT_H