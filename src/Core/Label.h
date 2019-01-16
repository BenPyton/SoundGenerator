/*
* @author PELLETIER Benoit
*
* @file Label.h
*
* @date 26/11/2018
*
* @brief Define a display text in SFML
*
*/

#ifndef _LABEL_H
#define _LABEL_H

#include <SFML/Graphics.hpp>
#include "AbstractUI.h"

using namespace std;

enum TextAlign {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
	NB_ALIGN
};

class Label : public AbstractUI
{
protected:
	sf::Text* m_text;
	TextAlign m_alignement;

public:
	// Constructor
	Label(int x, int y, int width, int height, UIStyle* style = &UIStyle::Default);
	virtual ~Label();

	// Accessors
	void setText(std::string text);
	std::string getText();
	void setCharacterSize(int size);
	int getCharacterSize();
	void setFont(sf::Font &font);
	const sf::Font* getFont();
	void setAlignement(TextAlign align);

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;


};

#endif // _LABEL_H