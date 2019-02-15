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

	m_connectionLines.setPrimitiveType(sf::PrimitiveType::Lines);
}

Pin::Pin(const Pin & _cp)
	: AbstractUI(_cp)
{
	m_pCircleShape = new sf::CircleShape(*_cp.m_pCircleShape);
	assert(nullptr != m_pCircleShape);
	m_line.resize(2);
	m_line.setPrimitiveType(sf::PrimitiveType::LinesStrip);

	m_connectionLines.setPrimitiveType(sf::PrimitiveType::Lines);

	m_connectionState = _cp.m_connectionState;
}

Pin::Pin(Pin && _p)
	: AbstractUI(std::move(_p)), m_pCircleShape(std::move(_p.m_pCircleShape))
{

	m_connectionState = _p.m_connectionState;
	m_line = _p.m_line;
	m_connectionLines = _p.m_connectionLines;
	m_pConnections = _p.m_pConnections;
	m_multiConnection = _p.m_multiConnection;

	_p.m_pCircleShape = nullptr;
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
	if (hovered(Input::GetMousePosition()))
	{
		if (Input::GetMouseButtonDown(sf::Mouse::Left) && _canConnect())
		{
			m_connectionState = ConnectionState::Pending;
		}

		if (Input::GetMouseButtonDown(sf::Mouse::Right))
		{
			_disconnect();
			for (int i = 0; i < m_pConnections.size(); i++)
			{
				m_pConnections[i]->_disconnect();
				vector<Pin*>::iterator it = find(m_pConnections[i]->m_pConnections.begin(), m_pConnections[i]->m_pConnections.end(), this);
				m_pConnections[i]->m_pConnections.erase(it);
				m_pConnections[i]->_updateConnections();
			}
			m_pConnections.clear();
			_updateConnections();
		}
	}

	if (Input::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (m_connectionState == ConnectionState::Pending)
		{
			// check if there is any Pin hovered by the mouse
			Pin* ui = UIManager::GetFirstHoveredUIOfType<Pin>(Input::GetMousePosition());
			if (ui != nullptr && ui->_canConnect())
			{
				vector<Pin*>::iterator it = find(m_pConnections.begin(), m_pConnections.end(), ui);
				if (it == m_pConnections.end() && _tryConnect(ui))
				{
					m_pConnections.push_back(ui);
					ui->m_pConnections.push_back(this);
					_updateConnections();
					ui->_updateConnections();
				}
			}
			m_connectionState = ConnectionState::None;
		}
	}

	if (m_connectionState != ConnectionState::Pending)
	{
		m_state = UI_NORMAL;
		if (m_pConnections.size() > 0)
		{
			m_connectionState = ConnectionState::Connected;
			m_state = UI_CLICKED;
		}
		else
		{
			m_connectionState = ConnectionState::None; 
			if (hovered(Input::GetMousePosition()))
				m_state = UI_HOVERED;
		}
	}
}

void Pin::_updateTransform()
{
	AbstractUI::_updateTransform();
	m_pCircleShape->setPosition(m_rect->getPosition());

	for (int i = 0; i < m_pConnections.size(); i++)
	{
		m_connectionLines[2*i].position = getRealPosition() + 0.5f * getRealSize();
		m_connectionLines[2*i+1].position = m_pConnections[i]->getRealPosition() + 0.5f * m_pConnections[i]->getRealSize();
	}

	if (m_connectionState == ConnectionState::Pending)
	{
		m_line[0].position = getRealPosition() + 0.5f * getRealSize();
		if (m_viewParent != nullptr)
		{
			m_line[1].position = m_viewParent->mapScreenPointToView(Input::GetMousePosition());
		}
		else
		{
			m_line[1].position = Input::GetMousePosition();
		}
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

	for (int i = 0; i < m_connectionLines.getVertexCount(); i++)
	{
		m_connectionLines[i].color = (*m_style)[UI_NORMAL].fgCol;
	}
}

void Pin::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_pCircleShape, states);
	target.draw(m_connectionLines, states);
	if (m_connectionState == ConnectionState::Pending)
	{
		target.draw(m_line, states);
	}
}

void Pin::_updateConnections()
{
	m_connectionLines.resize(2 * m_pConnections.size());
}

bool Pin::_canConnect()
{
	return m_multiConnection == true || (m_multiConnection == false && m_pConnections.empty());
}
