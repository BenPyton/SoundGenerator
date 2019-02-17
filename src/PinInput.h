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
#include "Core/InputField.h"

using namespace std;

class PinInput : public Pin
{
	friend class PinOutput;
private:
	ComponentInput* m_pInput = nullptr;
	sf::Text* m_pLabel = nullptr;
	InputField m_inputDefaultValue;

public:
	PinInput(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	PinInput(const PinInput& _pi);
	PinInput(PinInput&& _pi);
	virtual ~PinInput();
	PinInput& operator=(const PinInput& _pi) = delete;
	PinInput& operator=(PinInput&& _pi);
	void swap(PinInput& _other);

	void setInput(ComponentInput* _input);
	ComponentInput* getInput() { return m_pInput; }

	//PinInput& operator=(const PinInput&);

	virtual void update() override;

protected:
	virtual bool _tryConnect(Pin* _other) override;
	virtual void _disconnect() override;
	virtual void _updateStyle() override;
	virtual void _updateTransform() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // _PININPUT_H