/*
* @author PELLETIER Benoit
*
* @file Button.cpp
*
* @date 03/10/2018
*
* @brief Button class with SFML
*
*/

#include "stdafx.h"
#include "Button.h"
#include <cassert>


Button::Button(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style)
{
	m_text = new sf::Text();
	assert(nullptr != m_text);
}

Button::Button(const Button & _b)
	: AbstractUI(_b)
{
	m_text = new sf::Text(*_b.m_text);
	assert(nullptr != m_text);
}

Button::Button(Button && _b)
	: AbstractUI(_b)
{
	_b.swap(*this);
}

Button::~Button()
{
	if (nullptr != m_text)
	{
		delete m_text;
	}
}

Button & Button::operator=(Button && _b)
{
	_b.swap(*this);
	return *this;
}

void Button::swap(Button & _other)
{
	AbstractUI::swap(_other);
	std::swap(m_text, _other.m_text);
}

//void Button::SetNormalTexture(int x, int y, int w, int h)
//{
//	m_normalRect = sf::IntRect(x, y, w, h);
//}
//
//void Button::SetClickedTexture(int x, int y, int w, int h)
//{
//	m_clickedRect = sf::IntRect(x, y, w, h);
//}

void Button::setText(std::string text)
{
	if (nullptr != m_text)
	{
		m_text->setString(text);
	}
}

//void Button::update()
//{
//	AbstractUI::update();
//
//	/*sf::FloatRect textRect = m_text->getLocalBounds();
//	float posX = m_rect->getPosition().x + (int)(m_rect->getSize().x) / 2 - (int)(textRect.width + textRect.left) / 2;
//	float posY = m_rect->getPosition().y + (int)(m_rect->getSize().y) / 2 - m_text->getCharacterSize() * 0.6f;
//	m_text->setPosition(posX, posY);
//
//	m_text->setColor((*m_style)[m_state].fgCol);*/
//}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	AbstractUI::draw(target, states);
	target.draw(*m_text);
}

void Button::_updateTransform()
{
	AbstractUI::_updateTransform();
	sf::FloatRect textRect = m_text->getLocalBounds();
	float posX = m_rect->getPosition().x + (int)(m_rect->getSize().x) / 2 - (int)(textRect.width + textRect.left) / 2;
	float posY = m_rect->getPosition().y + (int)(m_rect->getSize().y) / 2 - m_text->getCharacterSize() * 0.6f;
	m_text->setPosition((int)posX, (int)posY);
}

void Button::_updateStyle()
{
	AbstractUI::_updateStyle();
	m_text->setFillColor((*m_style)[m_state].fgCol);
	m_text->setFont(*m_style->getFont());
	m_text->setCharacterSize(m_style->getCharacterSize());
}
