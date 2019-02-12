/*
* @author PELLETIER Benoit
*
* @file OutputComponent.h
*
* @date 12/02/2019
*
* @brief Define the output that generate the sound
*
*/

#ifndef _OUTPUTCOMPONENT_H
#define _OUTPUTCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class OutputComponent : public Component
{
private:
public:
	OutputComponent();
	virtual ~OutputComponent();

	virtual float getOutput(float _time) override;
};

#endif // _OUTPUTCOMPONENT_H