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
	: DraggableBox(x, y, width, height, style), m_output(0, 0, 10, style)
{
	m_pText = new sf::Text();
	assert(nullptr != m_pText);
	m_pText->setString("No component");
	m_output.setParent(this);
}

ComponentRenderer::~ComponentRenderer()
{
	if (nullptr != m_pText)
	{
		delete m_pText;
	}
}

void ComponentRenderer::setComponent(Component * _comp)
{
	m_pComponent = _comp;
	if (_comp != nullptr)
	{
		m_pText->setString(_comp->getName());
	}
	else
	{
		m_pText->setString("NULL");
	}
}

void ComponentRenderer::update()
{
	AbstractUI::update();
	m_output.setViewParent(m_viewParent);
	m_output.update();
}

void ComponentRenderer::_updateState()
{
	if (!m_output.hovered(Input::GetMousePosition()))
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
	m_output.setPosition(m_rect->getSize() * 0.5f);
}

void ComponentRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	DraggableBox::draw(target, states);
	target.draw(*m_pText, states);
	target.draw(m_output, states);
}
