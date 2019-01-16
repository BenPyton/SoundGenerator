/*
* @author PELLETIER Benoit
*
* @file Random.h
*
* @date 04/10/2018
*
* @brief Manage random numbers
*
*/

#ifndef _RANDOM_H
#define _RANDOM_H

#include <iostream>

using namespace std;

class Random
{
private:
	static Random m_instance;

	Random();
public:
	~Random();

	// Return a random integer in [min;max[
	static int Range(int min, int max);


};

#endif // _RANDOM_H