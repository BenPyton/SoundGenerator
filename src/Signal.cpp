/*
* @author PELLETIER Benoit
*
* @file Signal.cpp
*
* @date 16/01/2019
*
* @brief Class that define a signal
*
*/

#include "stdafx.h"
#include "Signal.h"

Signal::Signal()
{
	//m_vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
	m_sound.setLoop(true);
	m_sound.setBuffer(m_soundBuffer);
}

Signal::~Signal()
{
}

void Signal::setData(const vector<sf::Int16>& _data)
{ 
	m_samples = _data;
	m_soundBuffer.loadFromSamples(_data.data(), _data.size(), 1, 48000);
}
