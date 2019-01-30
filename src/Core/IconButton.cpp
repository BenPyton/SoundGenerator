/*
* @author PELLETIER Benoit
*
* @file IconButton.cpp
*
* @date 30/01/2019
*
* @brief Icon button class with SFML
*
*/

#include "stdafx.h"
#include "IconButton.h"
#include <cassert>


IconButton::IconButton(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style)
{
	m_iconTex = new sf::Texture();
	assert(nullptr != m_iconTex);
	m_iconTex->setSmooth(true);

	m_icon = new sf::RectangleShape();
	m_icon->setTexture(m_iconTex);
	m_icon->setOutlineColor(sf::Color::Transparent);
	m_icon->setOutlineThickness(0);
}

IconButton::~IconButton()
{
	if (nullptr != m_icon)
	{
		delete m_icon;
	}
	if (nullptr != m_iconTex)
	{
		delete m_iconTex;
	}
}

void IconButton::setIcon(std::string _fileName)
{
	if (nullptr != m_iconTex)
	{
		m_iconTex->loadFromFile(_fileName);
		m_icon->setTextureRect(sf::IntRect(0, 0, m_iconTex->getSize().x, m_iconTex->getSize().y));
	}
}

void IconButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	AbstractUI::draw(target, states);
	target.draw(*m_icon);
}

void IconButton::_updateTransform()
{
	AbstractUI::_updateTransform();
	m_icon->setPosition(m_rect->getPosition() + 0.5f * (m_rect->getSize() - m_iconSize));
	m_icon->setSize(m_iconSize);
}

void IconButton::_updateStyle()
{
	AbstractUI::_updateStyle();
	m_icon->setFillColor((*m_style)[m_state].fgCol);
}
