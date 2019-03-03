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

#include "GeneratorComponent.h"

class SinusComponent : public GeneratorComponent
{
private:

public:
	SinusComponent();
	virtual ~SinusComponent();

	virtual float getOutput(float _time) override;
};

#endif // _SINUSCOMPONENT_H