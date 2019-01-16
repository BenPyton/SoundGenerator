/*
* @author PELLETIER Benoit
*
* @file Input.cpp
*
* @date 04/10/2018
*
* @brief Manage SFML inputs
*
*/

#include "stdafx.h"
#include "Input.h"
#include "Window.h"

Input Input::m_instance = Input();

Input::Input()
{
	memset(m_keyPressed, 0, sf::Keyboard::Key::KeyCount * sizeof(bool));
	memset(m_keyReleased, 0, sf::Keyboard::Key::KeyCount * sizeof(bool));
	memset(m_key, 0, sf::Keyboard::Key::KeyCount * sizeof(bool));
	m_mouseScroll = 0.0f;
}

void Input::_Update(/*sf::RenderWindow & window*/)
{
	// Reset all pressed and release state
	memset(m_keyPressed, 0, sf::Keyboard::Key::KeyCount * sizeof(bool));
	memset(m_keyReleased, 0, sf::Keyboard::Key::KeyCount * sizeof(bool));
	memset(m_mousePressed, 0, sf::Mouse::ButtonCount * sizeof(bool));
	memset(m_mouseReleased, 0, sf::Mouse::ButtonCount * sizeof(bool));
	m_mouseScroll = 0.0f;
	m_enteredText.clear();

	sf::Event event;
	while (/*window.pollEvent(event)*/Window::GetWindow()->pollEvent(event))
	{

		switch (event.type)
		{
		case sf::Event::Closed:
			//window.close();
			Window::Close();
			break;
		case sf::Event::Resized:
			// update the view to the new size of the window
			//window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height))); 
			Window::GetWindow()->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			if (nullptr != Window::GetLayout())
			{
				Window::GetLayout()->setPosition(sf::Vector2f(0, 0));
				Window::GetLayout()->setSize(sf::Vector2f(event.size.width, event.size.height));
			}
			break;
		case sf::Event::MouseMoved:
			m_mousePos.x = event.mouseMove.x;
			m_mousePos.y = event.mouseMove.y;
			break;
		case sf::Event::MouseWheelScrolled:
			m_mouseScroll = event.mouseWheelScroll.delta;
			break;
		case sf::Event::KeyPressed:
			m_keyPressed[event.key.code] = true;
			m_key[event.key.code] = true;
			break;
		case sf::Event::KeyReleased:
			m_keyReleased[event.key.code] = true;
			m_key[event.key.code] = false;
			break;
		case sf::Event::MouseButtonPressed:
			m_mousePressed[event.mouseButton.button] = true;
			m_mouse[event.mouseButton.button] = true;
			break;
		case sf::Event::MouseButtonReleased:
			m_mouseReleased[event.mouseButton.button] = true;
			m_mouse[event.mouseButton.button] = false;
			break;
		case sf::Event::TextEntered:
			//if (event.text.unicode == 8) // backspace handling
			//{
			//	if (m_enteredText.getSize() > 0)
			//	{
			//		m_enteredText.erase(m_enteredText.getSize() - 1);
			//	}
			//}
			//else
			//{
			//	m_enteredText += event.text.unicode;
			//}
			m_enteredText += event.text.unicode;
			break;
		default:
			break;
		}
	}
}

Input::~Input()
{
}

void Input::Update(/*sf::RenderWindow & window*/)
{
	m_instance._Update(/*window*/);
}

bool Input::GetKeyDown(sf::Keyboard::Key key)
{
	return m_instance.m_keyPressed[key];
}

bool Input::GetKeyUp(sf::Keyboard::Key key)
{
	return  m_instance.m_keyReleased[key];
}

bool Input::GetKey(sf::Keyboard::Key key)
{
	return  m_instance.m_key[key];
}

bool Input::GetMouseButtonDown(sf::Mouse::Button button)
{
	return m_instance.m_mousePressed[button];
}

bool Input::GetMouseButton(sf::Mouse::Button button)
{
	return m_instance.m_mouse[button];
}

bool Input::GetMouseButtonUp(sf::Mouse::Button button)
{
	return m_instance.m_mouseReleased[button];
}

float Input::GetMouseWheelScroll()
{
	return m_instance.m_mouseScroll;
}

sf::Vector2f Input::GetMousePosition()
{
	return m_instance.m_mousePos;
}

sf::String Input::GetEnteredText()
{
	return m_instance.m_enteredText;
}
