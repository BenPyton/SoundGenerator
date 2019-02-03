/*
* @author PELLETIER Benoit
*
* @file PinInput.h
*
* @date 01/02/2019
*
* @brief Define an input pin
*
*/

#ifndef _PININPUT_H
#define _PININPUT_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pin.h"

using namespace std;

class PinInput : public Pin
{
	friend class PinOutput;
private:
	ComponentInput* m_pInput = nullptr;
	sf::Text* m_pLabel = nullptr;
public:
	PinInput(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	PinInput(const PinInput& _pi);
	~PinInput();

	void setInput(ComponentInput* _input);
	ComponentInput* getInput() { return m_pInput; }

protected:
	virtual bool _tryConnect(Pin* _other) override;
	virtual void _updateStyle() override;
	virtual void _updateTransform() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // _PININPUT_H