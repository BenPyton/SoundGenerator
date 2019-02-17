/*
* @author PELLETIER Benoit
*
* @file ComponentRenderer.cpp
*
* @date 22/01/2019
*
* @brief Define a component node renderer
*
*/

#include "stdafx.h"
#include "ComponentRenderer.h"
#include <cassert>

ComponentRenderer::ComponentRenderer(int x, int y, int width, int height, UIStyle& style)
	: DraggableBox(x, y, width, height, style), m_outputPin(0, 0, 10, style)
{
	m_pText = new sf::Text();
	assert(nullptr != m_pText);
	m_pText->setString("No component");
	m_outputPin.setParent(this);
}

ComponentRenderer::ComponentRenderer(ComponentRenderer && _cr)
	: DraggableBox(_cr), m_outputPin(_cr.m_outputPin)
{
	_cr.swap(*this);
}

ComponentRenderer::~ComponentRenderer()
{
	if (nullptr != m_pText)
	{
		delete m_pText;
	}
}

ComponentRenderer & ComponentRenderer::operator=(ComponentRenderer && _cr)
{
	_cr.swap(*this);
	return *this;
}

void ComponentRenderer::swap(ComponentRenderer & _other)
{
	DraggableBox::swap(_other);
	std::swap(m_pText, _other.m_pText);
	std::swap(m_outputPin, _other.m_outputPin);
	std::swap(m_inputPins, _other.m_inputPins);
}

void ComponentRenderer::setComponent(Component * _comp)
{
	m_pComponent = _comp;
	m_outputPin.setComponent(_comp);
	m_inputPins.clear();
	float heightSum = 30;
	if (_comp != nullptr)
	{
		m_pText->setString(_comp->getName());
		m_inputPins.resize(_comp->getInputCount(), PinInput(0, 0, 10, *m_style));
		//for (int i = 0; i < _comp->getInputCount(); i++)
		for (int i = 0; i < m_inputPins.size(); i++)
		{
			//PinInput pin(0, 0, 10, *m_style);
			//m_inputPins.emplace_back(std::move(pin));
			m_inputPins[i].setParent(this);
			m_inputPins[i].setInput(_comp->getInput(i));
			heightSum += (_comp->getInput(i)->getName().empty() ? 30 : 60);
		}
	}
	else
	{
		m_pText->setString("NULL");
	}
	m_size.y = heightSum;
}

void ComponentRenderer::update()
{
	DraggableBox::update();

	if (m_pComponent->hasOutput())
	{
		m_outputPin.setViewParent(m_viewParent);
		m_outputPin.update();
	}

	for (int i = 0; i < m_inputPins.size(); i++)
	{
		m_inputPins[i].setViewParent(m_viewParent);
		m_inputPins[i].update();
	}
}

void ComponentRenderer::_updateState()
{
	bool connectionHovered = false;
	for (int i = 0; i < m_inputPins.size(); i++)
	{
		connectionHovered |= m_inputPins[i].hovered(Input::GetMousePosition());
	}
	if (!connectionHovered && !m_outputPin.hovered(Input::GetMousePosition()))
	{
		DraggableBox::_updateState();
	}
}

void ComponentRenderer::_updateStyle()
{
	DraggableBox::_updateStyle();
	m_pText->setFont(*m_style->getFont());
	m_pText->setCharacterSize(m_style->getCharacterSize());
	m_pText->setFillColor((*m_style)[m_state].fgCol);
}

void ComponentRenderer::_updateTransform()
{
	DraggableBox::_updateTransform();
	m_pText->setPosition(m_rect->getPosition() + sf::Vector2f(5, 5));
	m_outputPin.setPosition(sf::Vector2f(m_rect->getSize().x - 30, m_rect->getSize().y*0.5f - 10));
	for (int i = 0; i < m_inputPins.size(); i++)
	{
		m_inputPins[i].setPosition(sf::Vector2f(10, 30 + 60 * i));
	}
}

void ComponentRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	DraggableBox::draw(target, states);
	target.draw(*m_pText, states);
	if (m_pComponent->hasOutput())
	{
		target.draw(m_outputPin, states);
	}

	for (int i = 0; i < m_inputPins.size(); i++)
	{
		target.draw(m_inputPins[i], states);
	}
}
