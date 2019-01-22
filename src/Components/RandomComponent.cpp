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

RandomComponent::RandomComponent()
{
	m_name = "Random";
}

RandomComponent::~RandomComponent()
{
}

float RandomComponent::getOutput(float _time)
{
	return 0.0f;
}