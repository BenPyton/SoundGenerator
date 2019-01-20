/*
* @author PELLETIER Benoit
*
* @file SinusComponent.h
*
* @date 19/01/2019
*
* @brief Implementation of sinusoidal generator component
*
*/

#ifndef _SINUSCOMPONENT_H
#define _SINUSCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class SinusComponent : public Component
{
private:
public:
	SinusComponent();
	~SinusComponent();


	virtual float getOutput(float _time) override;
};

#endif // _SINUSCOMPONENT_H