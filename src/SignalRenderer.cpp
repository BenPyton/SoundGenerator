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
	m_cursor.setPrimitiveType(sf::PrimitiveType::Lines);
	m_cursor.resize(2);


}

SignalRenderer::~SignalRenderer()
{
}

void SignalRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//AbstractUI::draw(target, states);
	target.draw(m_vertices, states);
	target.draw(m_cursor, states);
}

void SignalRenderer::_updateState()
{
	m_state = UI_NORMAL;
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

	if (m_signal != nullptr)
	{
		float playbackOffset = m_signal->getSound()->getPlayingOffset().asSeconds();
		float playbackCursor = 48000 * playbackOffset / m_signal->getSampleCount();
		//cout << "PlaybackOffset: " << playbackOffset << endl;
		m_cursor[0].position = m_rect->getPosition() + sf::Vector2f(playbackCursor * m_rect->getSize().x, 0);
		m_cursor[1].position = m_rect->getPosition() + sf::Vector2f(playbackCursor * m_rect->getSize().x, m_rect->getSize().y);
	}
}

void SignalRenderer::_updateStyle()
{
	AbstractUI::_updateStyle();
	for (int i = 0; i < m_vertices.getVertexCount(); i++)
	{
		m_vertices[i].color = (*m_style)[m_state].fgCol;
	}
	m_cursor[0].color = sf::Color::Yellow;//(*m_style)[m_state].fgCol;
	m_cursor[1].color = sf::Color::Yellow;//(*m_style)[m_state].fgCol;
}
