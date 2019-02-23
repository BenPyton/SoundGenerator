/*
* @author PELLETIER Benoit
*
* @file Window.h
*
* @date 31/10/2018
*
* @brief Singleton for SFML Window
*
*/

#ifndef _WINDOW_H
#define _WINDOW_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Layout.h"

using namespace std;

class Window
{
private:
	static Window m_instance;

	sf::RenderWindow* m_window;
	Layout* m_layout;
	int m_framerate;

	Window();
public:
	~Window();

	static void Create(sf::VideoMode mode, const sf::String title, sf::Uint32 style = 7U, const sf::ContextSettings& settings = sf::ContextSettings());
	static bool IsOpen();
	static void Update();
	static void Clear(const sf::Color& color = sf::Color(0, 0, 0, 255));
	static void Draw(const sf::RenderStates& states = sf::RenderStates::Default);
	static void Draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	static void Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
	static void Draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
	static void Draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states = sf::RenderStates::Default);
	static void Display();
	static void Close();

	static float GetWidth();
	static float GetHeight();
	static sf::Vector2f GetSize();

	static void SetLayout(Layout& layout);// { m_instance.m_layout = &layout; }
	static Layout* GetLayout() { return m_instance.m_layout; }

	static void SetFramerate(int framerate) { m_instance.m_framerate = framerate; }
	static int GetFramerate() { return m_instance.m_framerate; }

	static void SetView(sf::View view) { m_instance.m_window->setView(view); }
	static sf::View GetView() { return m_instance.m_window->getView(); }

	static sf::RenderWindow* GetWindow();
};

#endif // _WINDOW_H