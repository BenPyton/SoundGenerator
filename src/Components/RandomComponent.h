/*
* @author PELLETIER Benoit
*
* @file RandomComponent.h
*
* @date 22/01/2019
*
* @brief Implementation of a random signal generator
*
*/

#ifndef _RANDOMCOMPONENT_H
#define _RANDOMCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class RandomComponent : public Component
{
private:
public:
	RandomComponent();
	virtual ~RandomComponent();


	virtual float getOutput(float _time) override;

};

#endif // _RANDOMCOMPONENT_H