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
}

void View::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_rect, states);
	sf::View tmp = Window::GetView();
	Window::SetView(m_view);
	Layout::draw(target, states);
	Window::SetView(tmp);
}
