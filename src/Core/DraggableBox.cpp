/*
* @author PELLETIER Benoit
*
* @file DraggableBox.cpp
*
* @date 31/10/2018
*
* @brief Define a draggale box like a window on desktop
*
*/

#include "stdafx.h"
#include "DraggableBox.h"
#include "View.h"
#include "UIManager.h"

DraggableBox::DraggableBox(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style)
{
	m_dragged = false;
	m_mouseStart = sf::Vector2f(0, 0);
	m_startPos = sf::Vector2f(0, 0);
	m_snap = false;
	m_snapGrid = sf::Vector2f(1, 1);
}

DraggableBox::DraggableBox(const DraggableBox & _db)
	: AbstractUI(_db)
{
	m_dragged = _db.m_dragged;
	m_mouseStart = _db.m_mouseStart;
	m_startPos = _db.m_startPos;
	m_snap = _db.m_snap;
	m_snapGrid = _db.m_snapGrid;
}

DraggableBox::DraggableBox(DraggableBox && _db)
	: AbstractUI(_db)
{
	_db.swap(*this);
}

DraggableBox::~DraggableBox()
{
}

DraggableBox & DraggableBox::operator=(DraggableBox && _db)
{
	_db.swap(*this);
	return *this;
}

void DraggableBox::swap(DraggableBox & _other)
{
	AbstractUI::swap(_other);
	std::swap(m_dragged, _other.m_dragged);
	//std::swap(m_mouseOffset, _other.m_mouseOffset);
	std::swap(m_mouseStart, _other.m_mouseStart);
	std::swap(m_startPos, _other.m_startPos);
	std::swap(m_snap, _other.m_snap);
	std::swap(m_snapGrid, _other.m_snapGrid);
}

void DraggableBox::_updateState()
{
	if (m_dragged)
	{
		sf::Vector2f mouseNewPos;
		if (nullptr != m_viewParent)
		{
			mouseNewPos = m_viewParent->mapScreenPointToView(Input::GetMousePosition());// +m_mouseOffset;
		}
		else
		{
			mouseNewPos = Input::GetMousePosition();// + m_mouseOffset;
		}
		if (Input::GetMouseButtonUp(sf::Mouse::Left))
		{
			m_dragged = false;
			m_state = UIState::UI_NORMAL;
		}

		if (m_snap)
		{
			m_position = sf::Vector2f(
				floorf((m_startPos.x + (mouseNewPos.x - m_mouseStart.x)) / m_snapGrid.x) * m_snapGrid.x,
				floorf((m_startPos.y + (mouseNewPos.y - m_mouseStart.y)) / m_snapGrid.y) * m_snapGrid.y);
		}
		else
		{
			m_position = m_startPos + (mouseNewPos - m_mouseStart);
		}
	}
	else
	{
		if (click() && UIManager::GetFirstHoveredUI(Input::GetMousePosition()) == this)
		{
			if (nullptr != m_viewParent)
			{
				if (m_viewParent->hovered(Input::GetMousePosition()))
				{
					m_mouseStart = m_viewParent->mapScreenPointToView(Input::GetMousePosition());
					//m_mouseOffset = m_position - m_viewParent->mapScreenPointToView(Input::GetMousePosition());
				}
			}
			else
			{
				m_mouseStart = Input::GetMousePosition();
				//m_mouseOffset = m_position - Input::GetMousePosition();
			}

			m_startPos = m_position;
			m_dragged = true;
			m_state = UIState::UI_HOVERED;
		}
	}

	if (m_state != UIState::UI_HOVERED)
	{
		m_state = m_focused ? UIState::UI_FOCUSED : UIState::UI_NORMAL;
	}
}
