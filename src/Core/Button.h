/*
* @author PELLETIER Benoit
*
* @file Button.h
*
* @date 03/10/2018
*
* @brief Button class with SFML
*
*/

#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED


#include <SFML/Graphics.hpp>
#include "AbstractUI.h"

class Button : public AbstractUI
{
protected:
	sf::Text* m_text;

public:
	// Constructor
	Button(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	Button(const Button& _b);
	Button(Button&& _b);
	virtual ~Button();
	Button& operator=(const Button&) = delete;
	Button& operator=(Button&& _b);
	void swap(Button& _other);

	// Accessors
	void setText(std::string text);
	/*void SetNormalTexture(int x, int y, int w, int h);
	void SetClickedTexture(int x, int y, int w, int h);*/
	//virtual void update() override;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	//virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
};


#endif // BUTTON_H_INCLUDED