/*
* @author PELLETIER Benoit
*
* @file PinOutput.cpp
*
* @date 01/02/2019
*
* @brief Define an output pin
*
*/

#include "stdafx.h"
#include "PinInput.h"
#include "PinOutput.h"

PinOutput::PinOutput(int x, int y, int radius, UIStyle & style)
	: Pin(x, y, radius, style)
{
	m_multiConnection = true;
}

PinOutput::PinOutput(PinOutput && _po)
	: Pin(std::move(_po))
{
	m_pComponent = _po.m_pComponent;
	_po.m_pComponent = nullptr;
}

PinOutput::~PinOutput()
{
}

bool PinOutput::_tryConnect(Pin * _other)
{
	PinInput* input = dynamic_cast<PinInput*>(_other);
	if (input != nullptr && input->getInput() != nullptr && input->getInput()->getParent() != nullptr
		&& m_pComponent != nullptr && input->getInput()->getParent() != m_pComponent 
		&& input->getInput()->setComponent(m_pComponent))
	{
		return true;
	}

	return false;
}

