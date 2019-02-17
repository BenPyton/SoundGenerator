/*
* @author PELLETIER Benoit
*
* @file ComponentFactory.h
*
* @date 17/02/2019
*
* @brief Factory to create components from string names
*
*/

#ifndef _COMPONENTFACTORY_H
#define _COMPONENTFACTORY_H

#include <iostream>

class ComponentFactory
{
private:
	ComponentFactory() {}

public:
	// Create a component from it's name
	// Return nullptr if _compName doesn't corresponding to any component name
	static class Component* CreateComponent(const string& _compName);
};

#endif // _COMPONENTFACTORY_H