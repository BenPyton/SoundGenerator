/*
* @author PELLETIER Benoit
*
* @file Pin.h
*
* @date 29/01/2019
*
* @brief Define a connection point of a nodal interface component
*
*/

#ifndef _Pin_H
#define _Pin_H

#include <SFML/Graphics.hpp>
#include "Core/AbstractUI.h"
#include "Components\Component.h"

using namespace std;

class Pin : public AbstractUI
{
private:
	enum class ConnectionState {
		None,
		Pending,
		Connected
	};

protected:
	sf::CircleShape* m_pCircleShape = nullptr;
	ConnectionState m_connectionState;

	sf::VertexArray m_line;

	Pin* m_pConnection = nullptr;

public:
	Pin(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	Pin(const Pin& _cp);
	~Pin();

protected:
	virtual bool _tryConnect(Pin* _other) { return false; }
	virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // _Pin_H