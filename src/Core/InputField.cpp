/*
* @author PELLETIER Benoit
*
* @file InputField.cpp
*
* @date 06/10/2018
*
* @brief An UI input field
*
*/

#include "stdafx.h"
#include "InputField.h"
#include "Time.h"
#include <cassert>

InputField::InputField(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style)
{
	m_text = new sf::Text();
	assert(nullptr != m_text);
	m_str = "";
	m_placeholder = "";
	m_maxChar = 0;
	m_timer = 0;
}

InputField::~InputField()
{
	if (nullptr != m_text)
	{
		delete m_text;
	}
}

void InputField::setFont(sf::Font & font)
{
	m_text->setFont(font);
}

void InputField::setCharacterSize(int size)
{
	m_text->setCharacterSize(size);

}

void InputField::setMaxLength(int max)
{
	m_maxChar = max;
}

int InputField::getMaxLength()
{
	return m_maxChar;
}

void InputField::setPlaceholder(string placeholder)
{
	m_placeholder = placeholder;
}

string InputField::getPlaceholder()
{
	return m_placeholder.toAnsiString();
}

string InputField::getString()
{
	return m_str.toAnsiString();
}

float InputField::getFloat()
{
	return (float)atof(m_str.toAnsiString().c_str());
}

int InputField::getInt()
{
	return atoi(m_str.toAnsiString().c_str());
}

double InputField::getDouble()
{
	return atof(m_str.toAnsiString().c_str());
}

void InputField::setString(string str)
{
	m_str = str;
	_updateText();
}

void InputField::setFloat(float value)
{
	stringstream ss;
	ss << value;
	m_str = ss.str();
	_updateText();
}

void InputField::setInt(int value)
{
	stringstream ss;
	ss << value;
	m_str = ss.str();
	_updateText();
}

void InputField::setDouble(double value)
{
	stringstream ss;
	ss << value;
	m_str = ss.str();
	_updateText();
}


void InputField::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	AbstractUI::draw(target, states);
	target.draw(*m_text);
}

void InputField::_updateText()
{
	if (m_state == UIState::UI_FOCUSED)
	{
		// Get text entered during the frame
		m_timer += Time::GetDeltaTime();
		sf::String s = Input::GetEnteredText();
		for (int i = 0; i < s.getSize(); i++)
		{
			if (s[i] == 8) // backspace handler
			{
				if (m_str.getSize() > 0) // if empty we don't do anything
				{
					m_str.erase(m_str.getSize() - 1);
				}
			}
			else
			{
				if (m_maxChar <= 0 || (m_maxChar > 0 && m_str.getSize() < m_maxChar))
				{
					m_str += s[i];
				}
			}
		}
	}
	else
	{
		m_timer = 0;
	}

	if (((int)(m_timer * 1000) % 1000) > 500)
	{
		m_text->setString(m_str + "|");
	}
	else
	{
		m_text->setString(m_str);
	}

	//sf::FloatRect textRect = m_text.getLocalBounds();
	float posX = m_rect->getPosition().x + 10;
	float posY = m_rect->getPosition().y + m_rect->getSize().y / 2 - m_text->getCharacterSize() * 0.6f;
	m_text->setPosition(posX, posY);
}

void InputField::_updateState()
{
	if (m_enabled == false)
	{
		m_state = UIState::UI_DISABLED;
	}
	else if (m_state == UIState::UI_FOCUSED)
	{
		// lost focus
		if ((!hovered(Input::GetMousePosition()) && Input::GetMouseButtonDown(sf::Mouse::Left))
			|| Input::GetKeyDown(sf::Keyboard::Enter))
		{
			m_state = UIState::UI_NORMAL;
			// display placeholder if entered string is empty
			if (m_str.isEmpty())
			{
				m_text->setString(m_placeholder);
			}
		}
	}
	else
	{
		if (click())
		{
			m_state = UIState::UI_FOCUSED;
		}
		else if (hovered(Input::GetMousePosition()))
		{
			m_state = UIState::UI_HOVERED;
		}
		else
		{
			m_state = UIState::UI_NORMAL;
		}
	}
}

void InputField::_updateTransform()
{
	AbstractUI::_updateTransform();
	_updateText();
	//sf::FloatRect textRect = m_text.getLocalBounds();
	float posX = m_rect->getPosition().x + 10;
	float posY = m_rect->getPosition().y + m_rect->getSize().y / 2 - m_text->getCharacterSize() * 0.6f;
	m_text->setPosition((int)posX, (int)posY);
}

void InputField::_updateStyle()
{
	AbstractUI::_updateStyle();
	m_text->setFillColor((*m_style)[m_state].fgCol);
	m_text->setFont(*m_style->getFont());
	m_text->setCharacterSize(m_style->getCharacterSize());
}
