/*
* @author PELLETIER Benoit
*
* @file SawToothComponent.cpp
*
* @date 22/01/2019
*
* @brief Implementation of a sawtooth signal generator
*
*/

#include "stdafx.h"
#include "SawToothComponent.h"

SawToothComponent::SawToothComponent()
{
	m_name = "Saw Tooth";
}

SawToothComponent::~SawToothComponent()
{
}

float SawToothComponent::getOutput(float _time)
{
	return 0.0f;
}