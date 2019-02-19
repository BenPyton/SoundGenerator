/*
* @author PELLETIER Benoit
*
* @file ADSRComponent.h
*
* @date 18/02/2019
*
* @brief Define an ADSR (Attack, Decay, Sustain, Release) that modify the envelop of the signal
*
*/

#ifndef _ADSRCOMPONENT_H
#define _ADSRCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class ADSRComponent : public Component
{
private:
public:
	ADSRComponent();
	~ADSRComponent();


	virtual float getOutput(float _time) override;


};

#endif // _ADSRCOMPONENT_H