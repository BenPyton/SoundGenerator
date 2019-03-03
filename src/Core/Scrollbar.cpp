/*
* @author PELLETIER Benoit
*
* @file Scrollbar.cpp
*
* @date 23/02/2019
*
* @brief Define a scroll bar UI
*
*/

#include "stdafx.h"
#include "Scrollbar.h"
#include "Utils.h"

Scrollbar::Scrollbar(int x, int y, int w, int h, UIStyle& style)
	: AbstractUI(x, y, w, h, style)
{
	m_cursor = new sf::RectangleShape();
	m_startCursor = new sf::RectangleShape();
	m_endCursor = new sf::RectangleShape();
	m_nbStep = 100;
	m_currentStep = 0;
	m_viewedStep = 10;
	m_snap = false;
}

Scrollbar::~Scrollbar()
{
	delete m_cursor;
	delete m_startCursor;
	delete m_endCursor;
}

void Scrollbar::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	AbstractUI::draw(target, states);
	target.draw(*m_cursor, states);
	//target.draw(*m_endCursor, states);
	//target.draw(*m_startCursor, states);
}

void Scrollbar::_updateState()
{
	if (m_dragging)
	{
		sf::Vector2f deltaPos = Input::GetMousePosition() - m_mouseOffset;
		m_currentValue = m_startValue + (deltaPos.x / getRealSize().x) * m_nbStep;
		m_currentValue = clamp(m_currentValue, 0, m_nbStep - m_viewedStep);
		m_currentStep = (int)m_currentValue;

		if (Input::GetMouseButtonUp(sf::Mouse::Left))
		{
			m_dragging = false;
			m_state = UIState::UI_NORMAL;
		}
	}
	else if (m_cursor->getGlobalBounds().contains(Input::GetMousePosition()))
	{
		m_state = UIState::UI_HOVERED;
		if (Input::GetMouseButtonDown(sf::Mouse::Left))
		{
			m_state = UIState::UI_CLICKED;
			m_dragging = true;
			m_mouseOffset = Input::GetMousePosition();
			m_startValue = m_currentValue;
		}
	}
	else
	{
		m_state = UIState::UI_NORMAL;
	}
}

void Scrollbar::_updateTransform()
{
	AbstractUI::_updateTransform();

	float cursorSize = (m_viewedStep / (float)m_nbStep) * getRealSize().x;
	float cursorPos = ((m_snap ? m_currentStep : m_currentValue) / (float)m_nbStep) * getRealSize().x;

	m_cursor->setPosition(sf::Vector2f(cursorPos, 0) + getRealPosition());
	m_cursor->setSize(sf::Vector2f(cursorSize, getRealSize().y));

}

void Scrollbar::_updateStyle()
{
	AbstractUI::_updateStyle();

	m_rect->setFillColor((*m_style)[UIState::UI_NORMAL].bgCol);
	m_rect->setOutlineColor((*m_style)[UIState::UI_NORMAL].outCol);
	m_rect->setOutlineThickness((*m_style)[UIState::UI_NORMAL].outThick);

	m_cursor->setFillColor((*m_style)[m_state].bgCol);
	m_cursor->setOutlineColor((*m_style)[m_state].outCol);
	m_cursor->setOutlineThickness((*m_style)[m_state].outThick);
}
