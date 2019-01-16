/*
* @author PELLETIER Benoit
*
* @file Signal.h
*
* @date 16/01/2019
*
* @brief Class that define a signal
*
*/

#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

class Signal
{
private:
	/*sf::SoundBuffer buffer;
	sf::Sound sound(buffer);*/

	vector<sf::Int16> m_samples;

public:
	Signal();
	~Signal();

	void setData(const vector<sf::Int16>& _data) { m_samples = _data; }
	vector<sf::Int16> getData() { return m_samples; }

	sf::Int16 getValue(int _index) { return m_samples[_index]; }
	size_t getSampleCount() { return m_samples.size(); }
};

#endif // _SIGNAL_H