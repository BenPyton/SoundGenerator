/*
* @author PELLETIER Benoit
*
* @file SignalRenderer.h
*
* @date 16/01/2019
*
* @brief Renderer for a signal
*
*/

#ifndef _SIGNALRENDERER_H
#define _SIGNALRENDERER_H

#include <iostream>
#include "Core.h"
#include "Signal.h"

using namespace std;

class SignalRenderer : public AbstractUI
{
private:
	sf::VertexArray m_vertices;
	Signal* m_signal;

public:
	SignalRenderer(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	~SignalRenderer();

	void setSignal(Signal& _signal) { m_signal = &_signal; }

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	//virtual void _updateState() override;
	virtual void _updateTransform() override;
	virtual void _updateStyle() override;
};

#endif // _SIGNALRENDERER_H