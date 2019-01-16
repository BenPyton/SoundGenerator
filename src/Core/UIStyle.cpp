/*
* @author PELLETIER Benoit
*
* @file UIStyle.cpp
*
* @date 01/11/2018
*
* @brief Define style for ui
*
*/

#include "stdafx.h"
#include "UIStyle.h"

UIStyle UIStyle::Default;

UIStyle::UIStyle()
{
	mapStyle(UI_NORMAL, sf::Color::Black, sf::Color::White, sf::Color::Black, 1.0f);
	mapStyle(UI_HOVERED, sf::Color::Black, sf::Color::White, sf::Color::Black, 1.0f);
	mapStyle(UI_FOCUSED, sf::Color::Black, sf::Color::White, sf::Color::Black, 1.0f);
	mapStyle(UI_CLICKED, sf::Color::Black, sf::Color::White, sf::Color::Black, 1.0f);
	mapStyle(UI_DISABLED, sf::Color::Black, sf::Color::White, sf::Color::Black, 1.0f);
}

UIStyle::~UIStyle()
{
}

void UIStyle::mapStyle(UIState state, sf::Color foregroundColor, sf::Color backgroundColor, sf::Color outlineColor, float outlineThickness)
{
	m_styles[state].fgCol = foregroundColor;
	m_styles[state].bgCol = backgroundColor;
	m_styles[state].outCol = outlineColor;
	m_styles[state].outThick = outlineThickness;
}

UIStateStyle & UIStyle::operator[](UIState state)
{
	return m_styles[state];
}

void UIStyle::setFont(sf::Font & font, int charSize)
{
	m_font = &font;
	m_characterSize = charSize;
}

const sf::Font * UIStyle::getFont()
{
	return m_font;
}

int UIStyle::getCharacterSize()
{
	return m_characterSize;
}
