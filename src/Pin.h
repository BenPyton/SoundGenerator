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
protected:
	enum class ConnectionState {
		None,
		Pending,
		Connected
	};

protected:
	sf::CircleShape* m_pCircleShape = nullptr;
	ConnectionState m_connectionState;

	sf::VertexArray m_line;
	sf::VertexArray m_connectionLines;

	vector<Pin*> m_pConnections;

	bool m_multiConnection = false;

public:
	Pin(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	Pin(const Pin& _cp);
	Pin(Pin&& _p);
	~Pin();

protected:
	virtual bool _tryConnect(Pin* _other) { return false; }
	virtual void _disconnect() {}
	virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	void _updateConnections();
	bool _canConnect();
};

#endif // _Pin_H