/*
* @author PELLETIER Benoit
*
* @file AbstractUI.h
*
* @date 06/10/2018
*
* @brief Define a base class for UI elements
*
*/

#ifndef _ABSTRACTUI_H
#define _ABSTRACTUI_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "UIStyle.h"

using namespace std;

class AbstractUI : public sf::Drawable
{
protected:
	AbstractUI* m_parent;
	class View* m_viewParent;

	bool m_enabled;
	sf::RectangleShape* m_rect;

	UIState m_state;
	UIStyle* m_style;

	sf::Vector2f m_anchorMin, m_anchorMax;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Vector2f m_pivot;
	float m_marginTop, m_marginLeft, m_marginBottom, m_marginRight;

	bool m_horiStretch, m_vertStretch;

public:
	AbstractUI(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	AbstractUI(const AbstractUI& _aui);
	AbstractUI(AbstractUI&& _aui);
	virtual ~AbstractUI();

	// Accessors
	void setAnchor(sf::Vector2f anchor);

	void setAnchorMin(sf::Vector2f anchorMin);
	sf::Vector2f getAnchorMin();

	void setAnchorMax(sf::Vector2f anchorMax);
	sf::Vector2f getAnchorMax();

	void setPivot(sf::Vector2f pivot);
	sf::Vector2f getPivot();

	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition();

	void setStretch(bool horizontal, bool vertical);

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize();

	sf::Vector2f getRealPosition();
	sf::Vector2f getRealSize();

	void setEnabled(bool enabled);
	bool getEnabled();

	void setParent(AbstractUI* parent) { m_parent = parent; }
	AbstractUI* getParent() { return m_parent; }

	void setViewParent(View* parent) { m_viewParent = parent; }
	View* getViewParent() { return m_viewParent; }

	void setStyle(UIStyle* style) { m_style = style; }
	UIStyle* getStyle() { return m_style; }

	void setMargins(float left, float right, float top, float bottom);
	void getMargins(float& left, float& right, float& top, float& bottom);

	bool hovered(sf::Vector2f pos);
	bool click();
	virtual void update();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
	virtual void _updateState();
	virtual void _updateTransform();
	virtual void _updateStyle();
};

#endif // _ABSTRACTUI_H