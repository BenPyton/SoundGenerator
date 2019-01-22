/*
* @author PELLETIER Benoit
*
* @file View.h
*
* @date 03/11/2018
*
* @brief Define a container without overflow that can be zoomed and moved inside
*
*/

#ifndef _VIEW_H
#define _VIEW_H

#include <iostream>
#include "SFML/Graphics.hpp"
#include "Layout.h"

using namespace std;

class View : public Layout
{
private:
	sf::View m_view;
	float m_zoom;
	sf::Vector2f m_center;

	sf::VertexArray m_gridLine;

public:
	View(int x, int y, int width, int height, UIStyle& style);
	~View();

	sf::View getView() { return m_view; }

	float getZoom() { return m_zoom; }
	void setZoom(float zoom) { m_zoom = zoom; }

	sf::Vector2f getCenter() { return m_center; }
	void setCenter(sf::Vector2f center) { m_center = center; }

	sf::Vector2f mapScreenPointToView(sf::Vector2f point);
	sf::Vector2f mapScreenVectorToView(sf::Vector2f pointA, sf::Vector2f pointB);

	virtual bool add(AbstractUI& ui) override;
	virtual bool remove(AbstractUI& ui) override;

	virtual void update() override;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // _VIEW_H