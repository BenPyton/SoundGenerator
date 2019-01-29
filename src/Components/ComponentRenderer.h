/*
* @author PELLETIER Benoit
*
* @file ComponentRenderer.h
*
* @date 22/01/2019
*
* @brief Define a component node renderer
*
*/

#ifndef _COMPONENTRENDERER_H
#define _COMPONENTRENDERER_H

#include <iostream>
#include "../Core/DraggableBox.h"
#include "../ConnectionPoint.h"
#include "Component.h"

using namespace std;

class ComponentRenderer : public DraggableBox
{
private:
	Component* m_pComponent = nullptr;
	sf::Text* m_pText = nullptr;


	ConnectionPoint m_output;

public:
	ComponentRenderer(int x, int y, int width, int height, UIStyle& style = UIStyle::Default);
	~ComponentRenderer();

	void setComponent(Component* _comp);
	Component* getComponent() { return m_pComponent; }

	virtual void update() override;
protected:
	virtual void _updateState() override;
	virtual void _updateStyle() override;
	virtual void _updateTransform() override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif // _COMPONENTRENDERER_H