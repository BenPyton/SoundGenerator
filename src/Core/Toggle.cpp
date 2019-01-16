/*
* @author PELLETIER Benoit
*
* @file Toggle.cpp
*
* @date 07/10/2018
*
* @brief Define a toggle (checkbox) in SFML
*
*/

#include "stdafx.h"
#include "Toggle.h"

Toggle::Toggle(int x, int y, int width, int height, UIStyle& style)
	: Button(x, y, width, height, style)
{
	m_checked = false;
}

Toggle::~Toggle()
{
}

void Toggle::setChecked(bool checked)
{
	m_checked = checked;
}

bool Toggle::getChecked() const
{
	return m_checked;
}

void Toggle::_updateState()
{
	// toggle checked state when clicked
	if (click())
	{
		m_checked = !m_checked;
	}


	if (m_enabled == false)
	{
		m_state = UIState::UI_DISABLED;
	}
	else if (m_checked)
	{
		m_state = UIState::UI_CLICKED;
	}
	else if (hovered(Input::GetMousePosition()))
	{
		m_state = UIState::UI_HOVERED;
	}
	else
	{
		m_state = UIState::UI_NORMAL;
	}
}

//void Toggle::_updateTransform()
//{
//}
//
//void Toggle::_updateStyle()
//{
//}
//
//void Toggle::update()
//{
//	// toggle checked state when clicked
//	if (click())
//	{
//		m_checked = !m_checked;
//	}
//
//
//	if (m_enabled == false)
//	{
//		m_state = UIState::UI_DISABLED;
//	}
//	else if(m_checked)
//	{
//		m_state = UIState::UI_CLICKED;
//	}
//	else if (hovered(Input::GetMousePosition()))
//	{
//		m_state = UIState::UI_HOVERED;
//	}
//	else
//	{
//		m_state = UIState::UI_NORMAL;
//	}
//
//
//	m_rect->setFillColor((*m_style)[m_state].bgCol);
//	m_rect->setOutlineColor((*m_style)[m_state].outCol);
//	m_text->setFillColor((*m_style)[m_state].fgCol);
//}
