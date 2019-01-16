/*
* @author PELLETIER Benoit
*
* @file Time.cpp
*
* @date 03/10/2018
*
* @brief Manage frame time in SFML
*
*/

#include "stdafx.h"
#include "Time.h"

Time Time::m_instance = Time();

Time::Time()
{
	m_bufferIndex = 0;
	memset(m_deltaBuffer, 0, BUFFER_SIZE * sizeof(float));
	m_deltaTime = 0.0f;
	m_bufferAverage = 0.0f;
}

Time::~Time()
{
}


float Time::GetDeltaTime()
{
	return m_instance.m_deltaTime;
}

float Time::GetFps()
{
	return (m_instance.m_bufferAverage) != 0.0f ? 1.0f / m_instance.m_bufferAverage : 0.0f;
}

void Time::LockFramerate(int framerate)
{
	m_instance._LockFramerate(framerate);
}

// limit frame per second at framerate
void Time::_LockFramerate(int framerate)
{
	// Framerate must be positive not null
	assert(framerate > 0);

	// Sleep until next frame
	if (m_clock.getElapsedTime().asSeconds() < 1.0f / framerate)
	{
		sf::sleep(sf::seconds(1.0f / framerate) - m_clock.getElapsedTime());
	}
	m_deltaTime = m_clock.restart().asSeconds();

	// store frame time in buffer and increment buffer index
	m_deltaBuffer[m_bufferIndex] = m_deltaTime;
	m_bufferIndex = (m_bufferIndex + 1) % BUFFER_SIZE;

	// Get the average of the frametime buffer
	m_bufferAverage = 0.0f;
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		m_bufferAverage += m_deltaBuffer[i];
	}
	m_bufferAverage /= BUFFER_SIZE;
}
