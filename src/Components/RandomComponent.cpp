/*
* @author PELLETIER Benoit
*
* @file RandomComponent.cpp
*
* @date 22/01/2019
*
* @brief Implementation of a random signal generator
*
*/

#include "stdafx.h"
#include "RandomComponent.h"
#include "../Core/Random.h"

RandomComponent::RandomComponent()
{
	m_name = "Random";
}

RandomComponent::~RandomComponent()
{
}

float RandomComponent::getOutput(float _time)
{
	return 2 * (Random::Range(0, INT16_MAX) / (float)INT16_MAX) - 1;
}