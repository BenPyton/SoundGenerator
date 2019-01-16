/*
* @author PELLETIER Benoit
*
* @file Layout.cpp
*
* @date 01/11/2018
*
* @brief Define a ui container class
*
*/

#include "stdafx.h"
#include "Layout.h"

Layout::Layout(int x, int y, int width, int height)
	: AbstractUI(x, y, width, height)
{
}

Layout::~Layout()
{
}

bool Layout::add(AbstractUI & ui)
{
	if (contains(ui))
	{
		return false;
	}
	m_children.push_back(&ui);
	ui.setParent(this);
	return true;
}

bool Layout::remove(AbstractUI & ui)
{
	bool success = false;

	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end() && !success; ++it)
	{
		if ((*it) == &ui)
		{
			m_children.remove(&ui);
			ui.setParent(nullptr);
			success = true;
		}
	}
	return success;
}

size_t Layout::getChildCount()
{
	return m_children.size();
}

bool Layout::contains(AbstractUI & ui)
{
	bool success = false;

	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end() && !success; ++it)
	{
		if ((*it) == &ui)
		{
			success = true;
		}
	}
	return success;
}

void Layout::update()
{
	_updateTransform();
	/*_updateState();
	_updateStyle();*/
	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->update();
	}
}


void Layout::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//AbstractUI::draw(target, states);
	for (list<AbstractUI*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->draw(target, states);
	}
}
