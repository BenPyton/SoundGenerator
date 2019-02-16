/*
* @author PELLETIER Benoit
*
* @file PinInput.cpp
*
* @date 01/02/2019
*
* @brief Define an input pin
*
*/

#include "stdafx.h"
#include "PinOutput.h"
#include "PinInput.h"
#include <cassert>

PinInput::PinInput(int x, int y, int radius, UIStyle & style)
	:Pin(x, y, radius, style), m_pInput(nullptr)
{
	m_pLabel = new sf::Text();
	assert(nullptr != m_pLabel);
	m_pLabel->setString("No input");
}

PinInput::PinInput(const PinInput & _pi)
	: Pin(_pi)
{
	m_pLabel = new sf::Text(*_pi.m_pLabel);
	assert(nullptr != m_pLabel);
	m_pInput = _pi.m_pInput;
}

PinInput::PinInput(PinInput && _pi)
	: Pin(_pi)
{
	_pi.swap(*this);
}

PinInput::~PinInput()
{
	if (nullptr != m_pLabel)
	{
		delete m_pLabel;
	}
}

PinInput & PinInput::operator=(PinInput && _pi)
{
	_pi.swap(*this);
	return *this;
}

void PinInput::swap(PinInput & _other)
{
	Pin::swap(_other);
	std::swap(m_pLabel, _other.m_pLabel);
	std::swap(m_pInput, _other.m_pInput);
}

void PinInput::setInput(ComponentInput * _input)
{
	m_pInput = _input;
	if (_input != nullptr)
	{
		m_pLabel->setString(_input->getName());
	}
	else
	{
		m_pLabel->setString("NULL");
	}
}

bool PinInput::_tryConnect(Pin * _other)
{
	PinOutput* output = dynamic_cast<PinOutput*>(_other);
	if (output != nullptr && m_pInput != nullptr && m_pInput->getParent() != nullptr
		&& output->getComponent() != nullptr && output->getComponent() != m_pInput->getParent()
		&& m_pInput->setComponent(output->getComponent()))
	{
		return true;
	}

	return false;
}

void PinInput::_disconnect()
{
	m_pInput->setComponent(nullptr);
}

void PinInput::_updateStyle()
{
	Pin::_updateStyle();
	m_pLabel->setFont(*m_style->getFont());
	m_pLabel->setCharacterSize(m_style->getCharacterSize());
	m_pLabel->setFillColor((*m_style)[UI_NORMAL].fgCol);
}

void PinInput::_updateTransform()
{
	Pin::_updateTransform();
	m_pLabel->setPosition(m_rect->getPosition() + sf::Vector2f(2*m_pCircleShape->getRadius() + 5, 0));
}

void PinInput::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Pin::draw(target, states);
	target.draw(*m_pLabel, states);
}
