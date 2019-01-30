/*
* @author PELLETIER Benoit
*
* @file IconButton.h
*
* @date 30/01/2019
*
* @brief Icon button class with SFML
*
*/

#ifndef _ICONBUTTON_H
#define _ICONBUTTON_H


#include <SFML/Graphics.hpp>
#include "AbstractUI.h"

class IconButton : public AbstractUI
{
protected:
	sf::RectangleShape* m_icon;
	sf::Texture* m_iconTex;
	sf::Vector2f m_iconSize;

public:
	// Constructor
	IconButton(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	virtual ~IconButton();

	// 
	void setIcon(std::string _fileName);
	void setIconSize(float _w, float _h) { m_iconSize.x = _w; m_iconSize.y = _h; }
	sf::Vector2f getIconSize() { return m_iconSize; }

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
};


#endif // _ICONBUTTON_H