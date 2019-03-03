/*
* @author PELLETIER Benoit
*
* @file TriangleComponent.h
*
* @date 19/01/2019
*
* @brief Implementation of trangle signal generator component
*
*/

#ifndef _TRIANGLECOMPONENT_H
#define _TRIANGLECOMPONENT_H

#include "GeneratorComponent.h"

class TriangleComponent : public GeneratorComponent
{
private:
public:
	TriangleComponent();
	virtual ~TriangleComponent();

	virtual float getOutput(float _time) override;
};

#endif // _TRIANGLECOMPONENT_H