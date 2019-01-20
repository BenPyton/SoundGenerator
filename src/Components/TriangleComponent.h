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

#include <iostream>
#include "Component.h"

using namespace std;

class TriangleComponent : public Component
{
private:
public:
	TriangleComponent();
	~TriangleComponent();



	virtual float getOutput(float _time) override;

};

#endif // _TRIANGLECOMPONENT_H