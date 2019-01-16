/*
* @author PELLETIER Benoit
*
* @file VerticalLayout.h
*
* @date 02/11/2018
*
* @brief Define a vertical flow for ui
*
*/

#ifndef _VERTICALLAYOUT_H
#define _VERTICALLAYOUT_H

#include <iostream>
#include "Layout.h"

using namespace std;

class VerticalLayout : public Layout
{
private:

	float m_paddingLeft, m_paddingRight, m_paddingTop, m_paddingBottom;
	float m_spacing;

public:
	VerticalLayout(int x, int y, int width, int height);
	~VerticalLayout();

	void setPaddings(float left, float right, float top, float bottom);
	void getPaddings(float& left, float& right, float& top, float& bottom);

	void setSpacing(float spacing) { m_spacing = spacing; }
	float getSpacing() { return m_spacing; }

	virtual void update() override;
};

#endif // _VERTICALLAYOUT_H