/*
* @author PELLETIER Benoit
*
* @file VerticalLayout.cpp
*
* @date 02/11/2018
*
* @brief Define a vertical flow for ui
*
*/

#include "stdafx.h"
#include "VerticalLayout.h"

VerticalLayout::VerticalLayout(int x, int y, int width, int height)
	: Layout(x, y, width, height), m_paddingLeft(0), m_paddingRight(0), m_paddingTop(0), m_paddingBottom(0), m_spacing(0)
{
}

VerticalLayout::~VerticalLayout()
{
}

void VerticalLayout::setPaddings(float left, float right, float top, float bottom)
{
	m_paddingLeft = left;
	m_paddingRight = right;
	m_paddingTop = top;
	m_paddingBottom = bottom;
}

void VerticalLayout::getPaddings(float & left, float & right, float & top, float & bottom)
{
	left = m_paddingLeft;
	right = m_paddingRight;
	top = m_paddingTop;
	bottom = m_paddingBottom;
}

void VerticalLayout::update()
{
	// update first all children
	Layout::update();

	// override their positions
	float yoffset = m_paddingTop;
	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		AbstractUI* ui = *it;
		ui->setPosition(sf::Vector2f(0, yoffset));
		yoffset += m_spacing + ui->getSize().y;
		ui->setMargins(m_paddingLeft, m_paddingRight, 0, 0);
		ui->setAnchorMin(sf::Vector2f(0, 0));
		ui->setAnchorMax(sf::Vector2f(1, 0));
		//ui->setSize(sf::Vector2f(m_rect->getSize().x - m_paddingLeft - m_paddingRight, ui->getSize().y));
	}

}
