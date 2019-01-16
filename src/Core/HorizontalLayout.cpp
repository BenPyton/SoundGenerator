/*
* @author PELLETIER Benoit
*
* @file HorizontalLayout.cpp
*
* @date 25/11/2018
*
* @brief Define a horizontal flow for ui
*
*/

#include "stdafx.h"
#include "HorizontalLayout.h"

HorizontalLayout::HorizontalLayout(int x, int y, int width, int height)
	: Layout(x, y, width, height), m_paddingLeft(0), m_paddingRight(0), m_paddingTop(0), m_paddingBottom(0), m_spacing(0)
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::setPaddings(float left, float right, float top, float bottom)
{
	m_paddingLeft = left;
	m_paddingRight = right;
	m_paddingTop = top;
	m_paddingBottom = bottom;
}

void HorizontalLayout::getPaddings(float & left, float & right, float & top, float & bottom)
{
	left = m_paddingLeft;
	right = m_paddingRight;
	top = m_paddingTop;
	bottom = m_paddingBottom;
}

void HorizontalLayout::update()
{
	// update first all children
	Layout::update();

	// override their positions
	float xoffset = m_paddingLeft;
	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		AbstractUI* ui = *it;
		ui->setPosition(sf::Vector2f(xoffset, 0));
		xoffset += m_spacing + ui->getSize().x;
		ui->setMargins(0, 0, m_paddingTop, m_paddingBottom);
		ui->setAnchorMin(sf::Vector2f(0, 0));
		ui->setAnchorMax(sf::Vector2f(0, 1));
		//ui->setSize(sf::Vector2f(m_rect->getSize().x - m_paddingLeft - m_paddingRight, ui->getSize().y));
	}

}
