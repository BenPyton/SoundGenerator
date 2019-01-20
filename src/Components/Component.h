/*
* @author PELLETIER Benoit
*
* @file Component.h
*
* @date 16/01/2019
*
* @brief Define a component that modify a signal
*
*/

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <iostream>
#include <vector>
#include "SFML/System.hpp"

using namespace std;


class ComponentInput;

class Component
{
protected:
	vector<ComponentInput> m_inputs;

public:
	Component();
	~Component();

	virtual float getOutput(float _time) = 0;

	ComponentInput* getInput(int _index);
	ComponentInput* getInput(string _name);
};

class ComponentInput
{
private:
	Component* m_pComponent = nullptr;
	float m_defaultValue;
	string m_name;

public:
	ComponentInput(string _name);

	void setComponent(Component* _comp) { m_pComponent = _comp; }
	void setDefaultValue(float _value) { m_defaultValue = _value; }
	string getName() { return m_name; }
	float getValue(float _time);
};

#endif // _COMPONENT_H