/*
* @author PELLETIER Benoit
*
* @file RepeatComponent.h
*
* @date 18/02/2019
*
* @brief Define a component that repeat a signal
*
*/

#ifndef _REPEATCOMPONENT_H
#define _REPEATCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class RepeatComponent : public Component
{
private:
public:
	RepeatComponent();
	~RepeatComponent();


	virtual float getOutput(float _time) override;


};

#endif // _REPEATCOMPONENT_H