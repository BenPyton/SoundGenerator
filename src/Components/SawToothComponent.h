/*
* @author PELLETIER Benoit
*
* @file SawToothComponent.h
*
* @date 22/01/2019
*
* @brief Implementation of a sawtooth signal generator
*
*/

#ifndef _SAWTOOTHCOMPONENT_H
#define _SAWTOOTHCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class SawToothComponent : public Component
{
private:
public:
	SawToothComponent();
	virtual ~SawToothComponent();


	virtual float getOutput(float _time) override;

};

#endif // _SAWTOOTHCOMPONENT_H