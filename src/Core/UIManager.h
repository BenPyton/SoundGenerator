/*
* @author PELLETIER Benoit
*
* @file UIManager.h
*
* @date 29/01/2019
*
* @brief Manage all ui elements that inherit from AbstractUI
*
*/

#ifndef _UIMANAGER_H
#define _UIMANAGER_H

#include <vector>
#include "AbstractUI.h"

using namespace std;

class UIManager
{
	friend AbstractUI;

private:

	static UIManager m_instance;

	vector<AbstractUI*> m_uiList;


	UIManager();

	static void Add(AbstractUI& _ui);
	static void Remove(AbstractUI& _ui);

public:
	~UIManager();



	static AbstractUI* GetFirstHoveredUI(sf::Vector2f position);

	template<typename T>
	static T* GetFirstHoveredUIOfType(sf::Vector2f position)
	{
		T* hoveredUI = nullptr;
		for (vector<AbstractUI*>::reverse_iterator rit = m_instance.m_uiList.rbegin(); rit != m_instance.m_uiList.rend(); ++rit)
		{
			if ((*rit)->hovered(position))
			{
				cout << "Hovered: " << hoveredUI << endl;
				T* t = dynamic_cast<T*>(*rit);
				if (nullptr != t)
				{
					hoveredUI = t;
					break;
				}
			}
		}

		return hoveredUI;
	}

};

#endif // _UIMANAGER_H