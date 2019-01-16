/*
* @author PELLETIER Benoit
*
* @file Layout.h
*
* @date 01/11/2018
*
* @brief Define a ui container class
*
*/

#ifndef _LAYOUT_H
#define _LAYOUT_H

#include <iostream>
#include "AbstractUI.h"
#include <list>

using namespace std;

class Layout : public AbstractUI
{
protected:
	list<AbstractUI*> m_children;

public:
	Layout(int x, int y, int width, int height);
	~Layout();

	virtual bool add(AbstractUI& ui);
	virtual bool remove(AbstractUI& ui);
	virtual size_t getChildCount();
	virtual bool contains(AbstractUI& ui);

	virtual void update() override;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // _LAYOUT_H