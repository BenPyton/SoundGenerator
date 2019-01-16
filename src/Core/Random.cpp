/*
* @author PELLETIER Benoit
*
* @file Random.cpp
*
* @date 04/10/2018
*
* @brief Manage random numbers
*
*/

#include "stdafx.h"
#include "Random.h"
#include <time.h>

Random Random::m_instance;

Random::Random()
{
	srand((unsigned int)time(NULL));
}

Random::~Random()
{
}

int Random::Range(int min, int max)
{
	return rand() % (max - min) + min;
}
