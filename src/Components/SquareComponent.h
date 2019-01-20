/*
* @author PELLETIER Benoit
*
* @file SquareComponent.h
*
* @date 19/01/2019
*
* @brief Implementation of squared signal generator component
*
*/

#ifndef _SQUARECOMPONENT_H
#define _SQUARECOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class SquareComponent : public Component
{
private:
public:
	SquareComponent();
	~SquareComponent();


	virtual float getOutput(float _time) override { return 0.0f; };


};

#endif // _SQUARECOMPONENT_H