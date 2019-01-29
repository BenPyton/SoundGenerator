/*
* @author PELLETIER Benoit
*
* @file SignalRenderer.cpp
*
* @date 16/01/2019
*
* @brief Renderer for a signal
*
*/

#include "stdafx.h"
#include "SignalRenderer.h"

SignalRenderer::SignalRenderer(int x, int y, int width, int height, UIStyle& style)
	: AbstractUI(x, y, width, height, style)
{
	m_vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
}

SignalRenderer::~SignalRenderer()
{
}

void SignalRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//AbstractUI::draw(target, states);
	target.draw(m_vertices, states);
}

void SignalRenderer::_updateTransform()
{
	AbstractUI::_updateTransform();

	/*m_vertices.resize(m_signal->getSampleCount());
	cout << "Render vertex count: " << m_vertices.getVertexCount() << endl;
	if (m_vertices.getVertexCount() > 0)
	{
		float stepX = m_rect->getSize().x / (float)m_vertices.getVertexCount();
		float stepY = 1.0f / 0xffff;

		for (int i = 0; i < m_vertices.getVertexCount(); i++)
		{
			m_vertices[i].position.x = m_rect->getPosition().x + (float)i * stepX;
			m_vertices[i].position.y = m_rect->getPosition().y + m_rect->getSize().y * (0.5f - m_signal->getValue(i) * stepY);
		}
	}*/
	int nbVertex = min((int)m_signal->getSampleCount(), (int)m_rect->getSize().x);
	m_vertices.resize(nbVertex);
	//cout << "Render vertex count: " << m_vertices.getVertexCount() << endl;
	if (m_vertices.getVertexCount() > 0)
	{
		float sampleStep = m_signal->getSampleCount() / (float)m_vertices.getVertexCount();
		float stepX = m_rect->getSize().x / (float)m_vertices.getVertexCount();
		float stepY = 1.0f / 0xffff;

		for (int i = 0; i < m_vertices.getVertexCount(); i++)
		{
			m_vertices[i].position.x = m_rect->getPosition().x + (float)i * stepX;
			m_vertices[i].position.y = m_rect->getPosition().y + m_rect->getSize().y * (0.5f - m_signal->getValue(i * sampleStep) * stepY);
		}
	}
}

void SignalRenderer::_updateStyle()
{
	AbstractUI::_updateStyle();
	for (int i = 0; i < m_vertices.getVertexCount(); i++)
	{
		m_vertices[i].color = (*m_style)[m_state].fgCol;
	}
}
