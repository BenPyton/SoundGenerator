/*
* @author PELLETIER Benoit
*
* @file AddComponent.h
*
* @date 18/02/2019
*
* @brief Define a component that return the addition of two signals
*
*/

#ifndef _ADDCOMPONENT_H
#define _ADDCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class AddComponent : public Component
{
private:
public:
	AddComponent();
	~AddComponent();


	virtual float getOutput(float _time) override;


};

#endif // _ADDCOMPONENT_H