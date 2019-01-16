/*
* @author Benoit PELLETIER
*
* @file Animation.cpp
*
* @date 5/11/2017
*
* @brief Functions to animate a single double value
*
*/

#include "stdafx.h"
#include "Animation.h"


Animation::Animation(float min, float max, float duration, float delay)
{
	m_min = min;
	m_max = max;
	m_time = duration;
	m_play = false;
	m_ended = false;
	m_reverse = false;
	m_loop = false;
	m_mirror = false;
	m_value = min;
	m_currentTime = 0;
	m_delay = delay;
	m_currentDelay = 0;
}

void Animation::Update(float deltaTime)
{
	m_ended = false;

	if (m_play)
	{
		if (m_currentDelay <= m_delay)
		{
			m_currentDelay += deltaTime;
		}
		else 
		{
			if (m_reverse)
			{
				m_currentTime -= deltaTime;
				if (m_currentTime <= 0)
				{
					m_ended = true;
					if (m_mirror)
					{
						m_reverse = false;
						if (!m_loop)
						{
							m_play = false;
						}
					}
					else if (m_loop)
					{
						m_currentTime += m_time;
					}
				}
			}
			else
			{
				m_currentTime += deltaTime;
				if (m_currentTime >= m_time)
				{
					if (m_mirror)
					{
						m_reverse = true;
					}
					else
					{
						m_ended = true;
						if (m_loop)
						{
							m_currentTime -= m_time;
						}
						else
						{
							m_play = false;
						}
					}
				}
			}
		}

		if (m_time != 0)
		{
			m_value = Lerp((float)m_min, (float)m_max, m_currentTime / m_time);
		}
		else
		{
			m_value = 0;
		}
	}

	if (m_currentTime >= m_time)
	{
		m_value = m_max;
	}
	else if (m_currentTime <= 0)
	{
		m_value = m_min;
	}
}

void Animation::Play()
{
	m_play = true;
	m_currentTime = (m_reverse) ? m_time : 0;
	m_value = (m_reverse) ? m_max : m_min;
	m_currentDelay = 0;
}

void Animation::Stop()
{
	m_play = false;
}

void Animation::SetReverse(bool reverse)
{
	m_reverse = reverse;
}

void Animation::SetLoop(bool loop)
{
	m_loop = loop;
}

void Animation::SetMirror(bool mirror)
{
	m_mirror = mirror;
}


float Lerp(float min, float max, float t)
{
	return (max - min)*t + min;
}
