/*
* @author PELLETIER Benoit
*
* @file PinOutput.h
*
* @date 01/02/2019
*
* @brief Define an output pin
*
*/

#ifndef _PINOUTPUT_H
#define _PINOUTPUT_H

#include <iostream>
#include "Components/Component.h"
#include "Pin.h"

using namespace std;

class PinOutput : public Pin
{
	friend class PinInput;
private:
	Component* m_pComponent = nullptr;
public:
	PinOutput(int x, int y, int radius, UIStyle& style = UIStyle::Default);
	PinOutput(PinOutput&& _po);
	~PinOutput();


	void setComponent(Component* _comp) { m_pComponent = _comp; }
	Component* getComponent() { return m_pComponent; }

protected:
	virtual bool _tryConnect(Pin* _other) override;
};

#endif // _PINOUTPUT_H