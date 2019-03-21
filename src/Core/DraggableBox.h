/*
* @author PELLETIER Benoit
*
* @file DraggableBox.h
*
* @date 31/10/2018
*
* @brief Define a draggale box like a window on desktop
*
*/

#ifndef _DRAGGABLEBOX_H
#define _DRAGGABLEBOX_H

#include <iostream>
#include "AbstractUI.h"

using namespace std;

class DraggableBox : public AbstractUI
{
protected:
	bool m_dragged;
	sf::Vector2f m_mouseStart;
	sf::Vector2f m_startPos;

	bool m_snap = true;
	sf::Vector2f m_snapGrid = sf::Vector2f(50, 50);

public:
	DraggableBox(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	DraggableBox(const DraggableBox& _db);
	DraggableBox(DraggableBox&& _db);
	virtual ~DraggableBox();
	DraggableBox& operator=(const DraggableBox& _db) = delete;
	DraggableBox& operator=(DraggableBox&& _db);
	void swap(DraggableBox& _other);

protected:
	virtual void _updateState() override;
};

#endif // _DRAGGABLEBOX_H