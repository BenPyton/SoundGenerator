/*
* @author PELLETIER Benoit
*
* @file GeneratorComponent.h
*
* @date 03/03/2019
*
* @brief Base class for all generator component
*
*/

#ifndef _GENERATORCOMPONENT_H
#define _GENERATORCOMPONENT_H

#include <iostream>
#include "Component.h"

using namespace std;

class GeneratorComponent : public Component
{
protected:
	float m_temp;
	float m_prevTime;

public:
	GeneratorComponent();
	virtual ~GeneratorComponent();

	virtual void init() override;
};

#endif // _GENERATORCOMPONENT_H