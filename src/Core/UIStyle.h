/*
* @author PELLETIER Benoit
*
* @file UIStyle.h
*
* @date 01/11/2018
*
* @brief Define style for ui
*
*/

#ifndef _UISTYLE_H
#define _UISTYLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

enum UIState {
	UI_NORMAL, UI_HOVERED, UI_FOCUSED, UI_CLICKED, UI_DISABLED, UI_NB_STATE
};

struct UIStateStyle {
	sf::Color bgCol;
	sf::Color fgCol;
	sf::Color outCol;
	float outThick;
};

class UIStyle
{
private:
	UIStateStyle m_styles[UIState::UI_NB_STATE];
	sf::Font* m_font = nullptr;
	int m_characterSize = 0;

public:
	UIStyle();
	~UIStyle();

	void mapStyle(UIState state, sf::Color foregroundColor, sf::Color backgroundColor, sf::Color outlineColor, float outlineThickness);

	UIStateStyle& operator[](UIState state);

	void setFont(sf::Font& font, int charSize);
	const sf::Font* getFont();
	int getCharacterSize();

	static UIStyle Default;
};

#endif // _UISTYLE_H