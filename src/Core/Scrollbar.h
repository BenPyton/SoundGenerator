/*
* @author PELLETIER Benoit
*
* @file Scrollbar.h
*
* @date 23/02/2019
*
* @brief Define a scroll bar UI
*
*/

#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

#include <iostream>
#include "AbstractUI.h"
#include "Utils.h"

using namespace std;

class Scrollbar : public AbstractUI
{
private:

	int m_nbStep;
	int m_currentStep;
	int m_viewedStep;
	float m_currentValue;

	sf::RectangleShape* m_cursor;
	bool m_dragging;

	sf::RectangleShape* m_startCursor;
	bool m_draggingStart;

	sf::RectangleShape* m_endCursor;
	bool m_draggingEnd;

	sf::Vector2f m_mouseOffset;
	float m_startValue;

	bool m_snap;

public:
	Scrollbar(int x, int y, int w, int h, UIStyle& style = UIStyle::Default);
	virtual ~Scrollbar();

	float getCurrentValue() { return m_currentValue; }
	void setCurrentValue(float _value) { m_currentValue = clamp(_value, 0, m_nbStep - m_viewedStep); m_currentStep = (int)m_currentValue; }

	int getCurrentStep() { return m_currentStep; }
	void setCurrentStep(int _step) { setCurrentValue(_step); }

	float getNbStep() { return m_nbStep; }
	void setNbStep(int _nbStep) { m_nbStep = _nbStep; setCurrentValue(m_currentValue); }

	float getViewedStep() { return m_viewedStep; }
	void setViewedStep(int _viewedStep) { m_viewedStep = _viewedStep; setCurrentValue(m_currentValue); }

	bool getSnap() { return m_snap; }
	void setSnap(bool _snap) { m_snap = _snap; }

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
};

#endif // _SCROLLBAR_H