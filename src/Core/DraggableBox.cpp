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
}

DraggableBox::DraggableBox(const DraggableBox & _db)
	: AbstractUI(_db)
{
	m_dragged = _db.m_dragged;
	m_mouseOffset = _db.m_mouseOffset;
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
	std::swap(m_mouseOffset, _other.m_mouseOffset);
}

void DraggableBox::_updateState()
{
	if (m_dragged)
	{
		if (nullptr != m_viewParent)
		{
			m_position = m_viewParent->mapScreenPointToView((sf::Vector2f)Input::GetMousePosition()) + m_mouseOffset;
		}
		else
		{
			m_position = (sf::Vector2f)Input::GetMousePosition() + m_mouseOffset;
		}
		if (Input::GetMouseButtonUp(sf::Mouse::Left))
		{
			m_dragged = false;
			m_state = UIState::UI_NORMAL;
		}
	}
	else
	{
		if (click() && UIManager::GetFirstHoveredUI(Input::GetMousePosition()) == this)
		{
			if (nullptr != m_viewParent)
			{
				if (m_viewParent->hovered((sf::Vector2f)Input::GetMousePosition()))
				{
					m_mouseOffset = m_position - m_viewParent->mapScreenPointToView((sf::Vector2f)Input::GetMousePosition());
					m_dragged = true;
					m_state = UIState::UI_HOVERED;
				}
			}
			else
			{
				m_mouseOffset = m_position - (sf::Vector2f)Input::GetMousePosition();
				m_dragged = true;
				m_state = UIState::UI_HOVERED;
			}
		}
	}

	if (m_state != UIState::UI_HOVERED)
	{
		m_state = m_focused ? UIState::UI_FOCUSED : UIState::UI_NORMAL;
	}
}

//void DraggableBox::_updateTransform()
//{
//}

//void DraggableBox::update()
//{
//	if (m_dragged)
//	{
//		m_rect->setPosition((sf::Vector2f)Input::GetMousePosition() + m_mouseOffset);
//
//		if (Input::GetMouseButtonUp(sf::Mouse::Left))
//		{
//			m_dragged = false;
//			m_state = UIState::UI_NORMAL;
//		}
//	}
//	else
//	{
//		if (click())
//		{
//			m_dragged = true;
//			m_mouseOffset = m_rect->getPosition() - (sf::Vector2f)Input::GetMousePosition();
//			m_state = UIState::UI_HOVERED;
//		}
//	}
//
//
//	m_rect->setFillColor((*m_style)[m_state].bgCol);
//	m_rect->setOutlineColor((*m_style)[m_state].outCol);
//}
