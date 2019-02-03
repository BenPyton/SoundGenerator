/*
* @author PELLETIER Benoit
*
* @file Pin.cpp
*
* @date 29/01/2019
*
* @brief Define a connection point of a nodal interface component
*
*/

#include "stdafx.h"
#include "Pin.h"
#include "Core/View.h"
#include "Core/UIManager.h"
#include <cassert> 


Pin::Pin(int x, int y, int radius, UIStyle& style)
	: AbstractUI(x, y, 2 * radius, 2 * radius, style), m_connectionState(ConnectionState::None)
{
	m_pCircleShape = new sf::CircleShape(radius, 24);
	assert(nullptr != m_pCircleShape);

	m_line.resize(2);
	m_line.setPrimitiveType(sf::PrimitiveType::LinesStrip);
}

Pin::Pin(const Pin & _cp)
	: AbstractUI(_cp)
{
	m_pCircleShape = new sf::CircleShape(*_cp.m_pCircleShape);
	assert(nullptr != m_pCircleShape);
	m_line.resize(2);
	m_line.setPrimitiveType(sf::PrimitiveType::LinesStrip);

	m_connectionState = _cp.m_connectionState;
}

Pin::~Pin()
{
	if (nullptr != m_pCircleShape)
	{
		delete m_pCircleShape;
	}
}

void Pin::_updateState()
{
	m_state = UI_NORMAL;
	if (m_connectionState != ConnectionState::None)
	{
		m_state = UI_CLICKED;
	}

	if (hovered(Input::GetMousePosition()) && m_connectionState == ConnectionState::None)
	{
		if (Input::GetMouseButtonDown(sf::Mouse::Left))
		{
			m_connectionState = ConnectionState::Pending;
		}

		if (m_connectionState != ConnectionState::Pending)
		{
			m_state = UI_HOVERED;
		}
	}

	if (Input::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (m_connectionState == ConnectionState::Pending)
		{
			// check if there is any Pin hovered by the mouse
			Pin* ui = UIManager::GetFirstHoveredUIOfType<Pin>(Input::GetMousePosition());
			if (_tryConnect(ui))
			{
				m_connectionState = ConnectionState::Connected;
				ui->m_connectionState = ConnectionState::Connected;
			}
			else
			{
				m_connectionState = ConnectionState::None;
			}
		}
	}
}

void Pin::_updateTransform()
{
	AbstractUI::_updateTransform();
	m_pCircleShape->setPosition(m_rect->getPosition());

	m_line[0].position = getRealPosition() + 0.5f * getRealSize();
	switch (m_connectionState)
	{
	case ConnectionState::Connected:
		if (nullptr != m_pConnection)
		{
			m_line[1].position = m_pConnection->getRealPosition() + 0.5f * m_pConnection->getRealSize();
		}
		break;
	case ConnectionState::Pending:
		if (m_viewParent != nullptr)
		{
			m_line[1].position = m_viewParent->mapScreenPointToView(Input::GetMousePosition());
		}
		else
		{
			m_line[1].position = Input::GetMousePosition();
		}
		break;
	default: break;
	}

}

void Pin::_updateStyle()
{
	AbstractUI::_updateStyle();
	m_pCircleShape->setFillColor((*m_style)[m_state].bgCol);
	m_pCircleShape->setOutlineColor((*m_style)[m_state].fgCol);
	m_pCircleShape->setOutlineThickness(m_rect->getOutlineThickness());

	for (int i = 0; i < m_line.getVertexCount(); i++)
	{
		m_line[i].color = (*m_style)[UI_NORMAL].fgCol;
	}
}

void Pin::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_pCircleShape, states);
	if (m_connectionState != ConnectionState::None)
	{
		target.draw(m_line, states);
	}
}
