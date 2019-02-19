/*
* @author PELLETIER Benoit
*
* @file DelayComponent.h
*
* @date 18/02/2019
*
* @brief Define a component that delay a signal
*
*/

#ifndef _DELAYCOMPONENT_H
#define _DELAYCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class DelayComponent : public Component
{
private:
public:
	DelayComponent();
	~DelayComponent();


	virtual float getOutput(float _time) override;


};

#endif // _DELAYCOMPONENT_H