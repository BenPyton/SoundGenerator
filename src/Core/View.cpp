/*
* @author PELLETIER Benoit
*
* @file View.cpp
*
* @date 03/11/2018
*
* @brief Define a container without overflow that can be zoomed and moved inside
*
*/

#include "stdafx.h"
#include "Window.h"
#include "View.h"

View::View(int x, int y, int width, int height, UIStyle& style)
	:Layout(x, y, width, height)
{
	m_style = &style;
	m_zoom = 1.0f;

	m_gridLine.setPrimitiveType(sf::PrimitiveType::Lines);

}

View::~View()
{
}

sf::Vector2f View::mapScreenPointToView(sf::Vector2f point)
{
	sf::View tmp = Window::GetView();
	Window::SetView(m_view);
	sf::Vector2f viewPoint = Window::GetWindow()->mapPixelToCoords((sf::Vector2i)point);
	Window::SetView(tmp);
	return viewPoint;
}

sf::Vector2f View::mapScreenVectorToView(sf::Vector2f pointA, sf::Vector2f pointB)
{
	sf::View tmp = Window::GetView();
	Window::SetView(m_view);
	sf::Vector2f viewPoint = Window::GetWindow()->mapPixelToCoords((sf::Vector2i)pointB) - Window::GetWindow()->mapPixelToCoords((sf::Vector2i)pointA);
	Window::SetView(tmp);
	return viewPoint;
}

bool View::add(AbstractUI & ui)
{
	if (contains(ui))
	{
		return false;
	}
	m_children.push_back(&ui);
	ui.setViewParent(this);
	return true;
}

bool View::remove(AbstractUI & ui)
{
	bool success = false;

	for (list<AbstractUI*>::iterator it = m_children.begin(); it != m_children.end() && !success; ++it)
	{
		if ((*it) == &ui)
		{
			m_children.remove(&ui);
			ui.setViewParent(nullptr);
			success = true;
		}
	}
	return success;
}

void View::update()
{
	Layout::update();
	m_view.setViewport(sf::FloatRect(
		m_rect->getPosition().x / Window::GetSize().x,
		m_rect->getPosition().y / Window::GetSize().y,
		m_rect->getSize().x / Window::GetSize().x,
		m_rect->getSize().y / Window::GetSize().y
	));
	m_view.reset(sf::FloatRect(0, 0, m_rect->getSize().x, m_rect->getSize().y));
	m_view.zoom(m_zoom);
	m_view.setCenter(m_center);
	_updateStyle();




	//cout << "View width: " << m_view.getSize().x << " | height: " << m_view.getSize().y << " | Center: " << m_view.getCenter().x << ", " << m_view.getCenter().y << endl;
	// update grid display
	int cellSize = 50;
	int nbCellX = m_view.getSize().x / cellSize;
	int nbCellY = m_view.getSize().y / cellSize;
	m_gridLine.resize(2 * (nbCellX + nbCellY + 2));

	int offsetX = m_view.getCenter().x - m_view.getSize().x / 2;
	int offsetY = m_view.getCenter().y - m_view.getSize().y / 2;
	int gridOffsetX = offsetX / cellSize;
	int gridOffsetY = offsetY / cellSize;

	sf::Color gridColor(80, 80, 80);

	for (int i = 0; i < nbCellX + 1; i++)
	{
		m_gridLine[2 * i].position = sf::Vector2f((i + gridOffsetX) * cellSize, offsetY);
		m_gridLine[2 * i].color = gridColor;
		m_gridLine[2 * i + 1].position = sf::Vector2f((i + gridOffsetX) * cellSize, (nbCellY + 1) * cellSize + offsetY);
		m_gridLine[2 * i + 1].color = gridColor;
	}

	for (int i = 0; i < nbCellY + 1; i++)
	{
		m_gridLine[2 * (nbCellX + 1 + i)].position = sf::Vector2f(offsetX, (i + gridOffsetY) * cellSize);
		m_gridLine[2 * (nbCellX + 1 + i)].color = gridColor;
		m_gridLine[2 * (nbCellX + 1 + i) + 1].position = sf::Vector2f(offsetX + (nbCellX + 1) * cellSize, (i + gridOffsetY) * cellSize);
		m_gridLine[2 * (nbCellX + 1 + i) + 1].color = gridColor;
	}
}

void View::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_rect, states);
	sf::View tmp = Window::GetView();
	Window::SetView(m_view);
	target.draw(m_gridLine, states);
	Layout::draw(target, states);
	Window::SetView(tmp);
}
