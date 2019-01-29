/*
* @author PELLETIER Benoit
*
* @file Label.cpp
*
* @date 26/11/2018
*
* @brief Define a display text in SFML
*
*/

#include "stdafx.h"
#include "Label.h"


Label::Label(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style), m_text(nullptr)
{
	m_text = new sf::Text();
	m_alignement = ALIGN_LEFT;
}

Label::~Label()
{
	if (nullptr != m_text)
	{
		delete m_text;
	}
}

void Label::setText(std::string text)
{
	if (nullptr != m_text)
	{
		m_text->setString(text);
	}
}

std::string Label::getText()
{
	std::string str = "";
	if (nullptr != m_text)
	{
		str = m_text->getString();
	}
	return str;
}

//void Label::setCharacterSize(int size)
//{
//	if (nullptr != m_text)
//	{
//		m_text->setCharacterSize(size);
//	}
//}
//
//int Label::getCharacterSize()
//{
//	int size = 0;
//	if (nullptr != m_text)
//	{
//		size = m_text->getCharacterSize();
//	}
//	return size;
//}
//
//void Label::setFont(sf::Font & font)
//{
//	if (nullptr != m_text)
//	{
//		m_text->setFont(font);
//	}
//}
//
//const sf::Font* Label::getFont()
//{
//	const sf::Font* font = nullptr;
//	if (nullptr != m_text)
//	{
//		font = m_text->getFont();
//	}
//	return font;
//}

void Label::setAlignement(TextAlign align)
{
	m_alignement = align;
}

void Label::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//AbstractUI::draw(target, states);
	target.draw(*m_text);
}

void Label::_updateState()
{
	m_state = UIState::UI_NORMAL;
}

void Label::_updateTransform()
{
	AbstractUI::_updateTransform();
	sf::FloatRect textRect = m_text->getLocalBounds();
	float posX = 0;
	float posY = 0;
	switch (m_alignement)
	{
	case ALIGN_LEFT:
		posX = m_marginLeft;
		posY = (int)(m_rect->getSize().y) / 2 - m_text->getCharacterSize() * 0.6f;
		break;
	case ALIGN_CENTER:
		posX = (int)(m_rect->getSize().x) / 2 - (int)(textRect.width + textRect.left) / 2;
		posY = (int)(m_rect->getSize().y) / 2 - m_text->getCharacterSize() * 0.6f;
		break;
	case ALIGN_RIGHT:
		posX = (int)(m_rect->getSize().x) - (int)(textRect.width + textRect.left) - m_marginRight;
		posY = (int)(m_rect->getSize().y) / 2 - m_text->getCharacterSize() * 0.6f;
		break;
	default:
		break;
	}

	m_text->setPosition(m_rect->getPosition() + sf::Vector2f((int)posX, (int)posY));
}

void Label::_updateStyle()
{
	AbstractUI::_updateStyle();
	m_text->setFillColor((*m_style)[m_state].fgCol);
	m_text->setFont(*m_style->getFont());
	m_text->setCharacterSize(m_style->getCharacterSize());
}

