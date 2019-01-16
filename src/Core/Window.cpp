/*
* @author PELLETIER Benoit
*
* @file Window.cpp
*
* @date 31/10/2018
*
* @brief Singleton for SFML Window
*
*/

#include "stdafx.h"
#include "Window.h"
#include "Input.h"
#include "Time.h"
#include <cassert>

Window Window::m_instance = Window();

Window::Window()
	: m_window(nullptr), m_layout(nullptr), m_framerate(60)
{
}

Window::~Window()
{
	if (nullptr != m_window)
	{
		delete m_window;
		m_window = nullptr;
	}
}

void Window::Create(sf::VideoMode mode, const sf::String title, sf::Uint32 style, const sf::ContextSettings& settings)
{
	assert(nullptr == m_instance.m_window);
	m_instance.m_window = new sf::RenderWindow(mode, title, style, settings);
}

bool Window::IsOpen()
{
	assert(nullptr != m_instance.m_window);
	return m_instance.m_window->isOpen();
}

void Window::Update()
{
	assert(nullptr != m_instance.m_window);

	Time::LockFramerate(m_instance.m_framerate);
	Input::Update();
	if (nullptr != m_instance.m_layout)
	{
		m_instance.m_layout->update();
	}
}

void Window::Clear(const sf::Color& color)
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->clear(color);
}

void Window::Draw(const sf::RenderStates & states)
{
	assert(nullptr != m_instance.m_window);
	if (nullptr != m_instance.m_layout)
	{
		m_instance.m_window->draw(*m_instance.m_layout, states);
	}
}

void Window::Draw(const sf::Drawable & drawable, const sf::RenderStates & states)
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->draw(drawable, states);
}

void Window::Draw(const sf::Vertex * vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates & states)
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->draw(vertices, vertexCount, type, states);
}

void Window::Draw(const sf::VertexBuffer & vertexBuffer, const sf::RenderStates & states)
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->draw(vertexBuffer, states);
}

void Window::Draw(const sf::VertexBuffer & vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates & states)
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->draw(vertexBuffer, firstVertex, vertexCount, states);
}

void Window::Display()
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->display();
}

void Window::Close()
{
	assert(nullptr != m_instance.m_window);
	m_instance.m_window->close();
	/*if (nullptr != m_instance.m_window)
	{
		delete m_instance.m_window;
		m_instance.m_window = nullptr;
	}*/
}

float Window::GetWidth()
{
	assert(nullptr != m_instance.m_window);
	return m_instance.m_window->getSize().x;
}

float Window::GetHeight()
{
	assert(nullptr != m_instance.m_window);
	return m_instance.m_window->getSize().y;
}

sf::Vector2f Window::GetSize()
{
	assert(nullptr != m_instance.m_window);
	return (sf::Vector2f)m_instance.m_window->getSize();
}

sf::RenderWindow * Window::GetWindow()
{
	assert(nullptr != m_instance.m_window);
	return m_instance.m_window;
}
