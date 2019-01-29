/*
* @author PELLETIER Benoit
*
* @file ConnectionPoint.h
*
* @date 29/01/2019
*
* @brief Define a connection point of a nodal interface component
*
*/

#ifndef _CONNECTIONPOINT_H
#define _CONNECTIONPOINT_H

#include <SFML/Graphics.hpp>
#include "Core/AbstractUI.h"

using namespace std;

class ConnectionPoint : public AbstractUI
{
private:
	enum class ConnectionState {
		None,
		Pending,
		Connected
	};

	sf::CircleShape* m_pCircleShape = nullptr;
	ConnectionState m_connectionState;

	sf::VertexArray m_line;

	ConnectionPoint* m_pConnection = nullptr;

public:
	ConnectionPoint(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	~ConnectionPoint();

protected:
	virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // _CONNECTIONPOINT_H