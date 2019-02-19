/*
* @author PELLETIER Benoit
*
* @file MultiplyComponent.h
*
* @date 18/02/2019
*
* @brief Define a component that multiply tow signals
*
*/

#ifndef _MULTIPLYCOMPONENT_H
#define _MULTIPLYCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class MultiplyComponent : public Component
{
private:
public:
	MultiplyComponent();
	~MultiplyComponent();



	virtual float getOutput(float _time) override;

};

#endif // _MULTIPLYCOMPONENT_H