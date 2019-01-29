/*
* @author PELLETIER Benoit
*
* @file UIManager.cpp
*
* @date 29/01/2019
*
* @brief Manage all ui elements that inherit from AbstractUI
*
*/

#include "stdafx.h"
#include "UIManager.h"
#include <iostream>

UIManager UIManager::m_instance = UIManager();

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

AbstractUI * UIManager::GetFirstHoveredUI(sf::Vector2f position)
{
	AbstractUI* hoveredUI = nullptr;
	for (vector<AbstractUI*>::reverse_iterator rit = m_instance.m_uiList.rbegin(); rit != m_instance.m_uiList.rend(); ++rit)
	{
		if ((*rit)->hovered(position))
		{
			hoveredUI = *rit;
			cout << "Hovered: " << hoveredUI << endl;
			break;
		}
	}

	return hoveredUI;
}

void UIManager::Add(AbstractUI & _ui)
{
	vector<AbstractUI*>::iterator it = find(m_instance.m_uiList.begin(), m_instance.m_uiList.end(), &_ui);
	if (it == m_instance.m_uiList.end())
	{
		m_instance.m_uiList.push_back(&_ui);
	}
}

void UIManager::Remove(AbstractUI & _ui)
{
	vector<AbstractUI*>::iterator it = find(m_instance.m_uiList.begin(), m_instance.m_uiList.end(), &_ui);
	if (it != m_instance.m_uiList.end())
	{
		m_instance.m_uiList.erase(it);
	}
}
