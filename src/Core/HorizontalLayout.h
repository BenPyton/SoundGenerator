/*
* @author PELLETIER Benoit
*
* @file HorizontalLayout.h
*
* @date 02/25/2018
*
* @brief Define a horizontal flow for ui
*
*/

#ifndef _HORIZONTALLAYOUT_H
#define _HORIZONTALLAYOUT_H

#include <iostream>
#include "Layout.h"

using namespace std;

class HorizontalLayout : public Layout
{
private:

	float m_paddingLeft, m_paddingRight, m_paddingTop, m_paddingBottom;
	float m_spacing;

public:
	HorizontalLayout(int x, int y, int width, int height);
	~HorizontalLayout();

	void setPaddings(float left, float right, float top, float bottom);
	void getPaddings(float& left, float& right, float& top, float& bottom);

	void setSpacing(float spacing) { m_spacing = spacing; }
	float getSpacing() { return m_spacing; }

	virtual void update() override;
};

#endif // _HORIZONTALLAYOUT_H