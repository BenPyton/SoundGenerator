#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Core.h" // custom classes and functions (like Input, Button, etc.)
#include "Signal.h"
#include "SignalRenderer.h"
#include "Components.h"
#include "Components/ComponentRenderer.h"
#include "JSON/FileManager.h"

#define SAMPLERATE 48000

using namespace std;

int main()
{
	Window::Create(sf::VideoMode(800, 600), "Sound Generator");

	// Load font
	sf::Font font;
	if (!font.loadFromFile("../../../data/font/consola.ttf"))
	{
		printf("Error when loading font");
	}

	sf::Color lightGrey(200, 200, 200);
	sf::Color darkGrey(80, 80, 80);

	UIStyle style;
	style.setFont(font, 16);
	style.mapStyle(UI_NORMAL, lightGrey, sf::Color::Black, lightGrey, 1.0f);
	style.mapStyle(UI_HOVERED, lightGrey, sf::Color(50, 50, 50), lightGrey, 3.0f);
	style.mapStyle(UI_CLICKED, sf::Color::Black, lightGrey, lightGrey, 1.0f);
	style.mapStyle(UI_DISABLED, darkGrey, sf::Color::Black, darkGrey, 1.0f);
	style.mapStyle(UI_FOCUSED, lightGrey, sf::Color::Blue, lightGrey, 1.0f);


	// Button Example
	Button button(10, -10, 180, 30, style);
	button.setText("Generate");
	button.setMargins(10, 10, 10, 10);
	button.setAnchorMin(sf::Vector2f(0.0f, 1.0f));
	button.setAnchorMax(sf::Vector2f(0.0f, 1.0f));
	button.setPivot(sf::Vector2f(0.0f, 1.0f));


	Label labelDuration(0, 0, 100, 30, style);
	labelDuration.setText("Duration");
	labelDuration.setAlignement(TextAlign::ALIGN_RIGHT);


	string compList[10] =
	{
		"Sinusoidal",
		"Square",
		"Triangle",
		"Saw Tooth",
		"Random",
		"Add",
		"Multiply",
		"Repeat",
		"Delay",
		"ADSR"
	};

	VerticalLayout vLayout(0, 100, 200, 200);
	vLayout.setAnchorMin(sf::Vector2f(0, 0));
	vLayout.setAnchorMax(sf::Vector2f(0, 1));
	vLayout.setMargins(10, 10, 150, 10);
	vLayout.setSpacing(10);
	vLayout.setPaddings(10, 10, 10, 10);

	vector<Button> compBtns;
	compBtns.resize(10, Button(0, 0, 0, 30, style));
	for (int i = 0; i < compBtns.size(); i++)
	{
		compBtns[i].setText(compList[i]);
		vLayout.add(compBtns[i]);
	}

	HorizontalLayout toolLayout(10, 10, 0, 30);
	toolLayout.setAnchorMin(sf::Vector2f(0, 0));
	toolLayout.setAnchorMax(sf::Vector2f(1, 0));
	toolLayout.setMargins(10, 10, 10, 10);
	toolLayout.setSpacing(10);

	// View
	View view(0, 0, 0, 0, style);
	view.setAnchorMin(sf::Vector2f(0, 0));
	view.setAnchorMax(sf::Vector2f(1, 1));
	view.setMargins(200, 10, 150, 10);

	// Window layout
	Layout rootLayout(0, 0, Window::GetWidth(), Window::GetHeight());
	Window::SetLayout(rootLayout);
	rootLayout.add(button);
	rootLayout.add(vLayout);
	rootLayout.add(view);
	rootLayout.add(toolLayout);


	IconButton btnLoad(10, 10, 30, 30, style);
	btnLoad.setIconSize(32, 32);
	btnLoad.setIcon("../../../data/Images/icon_open.png");

	IconButton btnSave(10, 10, 30, 30, style);
	btnSave.setIconSize(32, 32);
	btnSave.setIcon("../../../data/Images/icon_save.png");

	IconButton btnStart(10, 10, 30, 30, style);
	btnStart.setIconSize(32, 32);
	btnStart.setIcon("../../../data/Images/icon_start.png");

	IconButton btnPlay(10, 10, 30, 30, style);
	btnPlay.setIconSize(32, 32);
	btnPlay.setIcon("../../../data/Images/icon_play.png");

	IconButton btnPause(10, 10, 30, 30, style);
	btnPause.setIconSize(32, 32);
	btnPause.setIcon("../../../data/Images/icon_pause.png");

	IconButton btnStop(10, 10, 30, 30, style);
	btnStop.setIconSize(32, 32);
	btnStop.setIcon("../../../data/Images/icon_stop.png");

	IconButton btnEnd(10, 10, 30, 30, style);
	btnEnd.setIconSize(32, 32);
	btnEnd.setIcon("../../../data/Images/icon_end.png");

	IconButton btnExport(10, 10, 30, 30, style);
	btnExport.setIconSize(32, 32);
	btnExport.setIcon("../../../data/Images/icon_export.png");

	Label labelVolume(0, 0, 100, 30, style);
	labelVolume.setText("Volume");
	labelVolume.setAlignement(TextAlign::ALIGN_RIGHT);

	// InputField Example
	InputField inputDuration(10, 90, 150, 30, style);
	inputDuration.setMaxLength(10);
	inputDuration.setFloat(0.1f);
	inputDuration.setPlaceholder("Test");

	InputField inputVolume(90, 50, 70, 30, style);
	inputVolume.setMaxLength(5);
	inputVolume.setFloat(10.0f);
	inputVolume.setPlaceholder("Volume");


	toolLayout.add(btnLoad);
	toolLayout.add(btnSave);
	toolLayout.add(btnExport);
	toolLayout.add(btnStart);
	toolLayout.add(btnPlay);
	toolLayout.add(btnPause);
	toolLayout.add(btnStop);
	toolLayout.add(btnEnd);
	toolLayout.add(labelVolume);
	toolLayout.add(inputVolume);
	toolLayout.add(labelDuration);
	toolLayout.add(inputDuration);


	vector<sf::Int16> samples;
	/*sf::VertexArray vertices;
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);*/

	Signal signal;
	SignalRenderer signalRenderer(10, 50, 100, 80, style);
	signalRenderer.setSignal(signal);
	signalRenderer.setAnchorMin(sf::Vector2f(0, 0));
	signalRenderer.setAnchorMax(sf::Vector2f(1, 0));
	signalRenderer.setMargins(200, 10, 10, 10);
	rootLayout.add(signalRenderer);

	/*float fadein_first = 0.2;
	float fadein_last = 0.6f;
	float fadeout_first = 0.4f;
	float fadeout_last = 0.8f;*/

	sf::Vector2f prevMousePos;


	vector<Component*> components;
	vector<ComponentRenderer*> compRenderers;

	// Component renderers
	OutputComponent output;
	ComponentRenderer outputRenderer(-100, -200, 150, 100, style);
	outputRenderer.setComponent(&output);
	view.add(outputRenderer);


	bool playing = false;
	bool paused = false;
	float maxZoomIn = 0.1f;
	float maxZoomOut = 10.0f;


	ComponentRenderer* selectedComp = nullptr;

	// ///////////////////////////// APPLICATION LOOP
	while (Window::IsOpen())
	{
		prevMousePos = Input::GetMousePosition();
		Window::Update();

		// ///////////////////////// GAME LOGIC
		if (Input::GetKeyDown(sf::Keyboard::Escape))
		{
			Window::Close();
		}

		Window::GetWindow()->setTitle("Sound Generator | FPS: " + floatToStr(Time::GetFps(), 0));

		if (button.click())
		{
			samples.clear();
			//vertices.clear();

			float duration = inputDuration.getFloat();

			//float step = Window::GetWidth() / (duration * SAMPLERATE);
			//float frequency = input.getInt();
			//float attenuation = 0.5f;

			sf::Uint64 totalSample = duration * SAMPLERATE;

			for (int i = 0; i < totalSample; i++)
			{
				//samples.push_back(Random::Range(-128, 128));
				float x = (float)i / SAMPLERATE;
				//attenuation = 1.0f;
				/*if (x > fadeout_last || x < fadein_first)
				{
					attenuation = 0;
				}
				else {
					if (x >= fadein_first && x <= fadein_last)
					{
						attenuation *= mapValue(x, fadein_first, fadein_last, 0, 1);
					}
					if (x >= fadeout_first && x <= fadeout_last)
					{
						attenuation *= mapValue(x, fadeout_first, fadeout_last, 1, 0);
					}
				}*/
				//samples.push_back(0x8000 * 0.999f * attenuation * sinf(frequency * 2 * 3.1415926f * x));

				samples.push_back(0x7FFF * clamp(output.getOutput(x), -1.0f, 1.0f));
				//vertices.append(sf::Vertex(sf::Vector2f(i * step, 0.5f * Window::GetWidth() + mapValue(samples[i], -512, 512, -200, 200)), sf::Color::White));
			}
			//buffer.loadFromSamples(samples.data(), SAMPLERATE, 1, SAMPLERATE);
			signal.setData(samples);
			cout << "Signal sample count: " << signal.getSampleCount() << endl;
		}


		for (int i = 0; i < compBtns.size(); i++)
		{
			if (compBtns[i].click())
			{
				components.push_back(ComponentFactory::CreateComponent(compList[i]));
				compRenderers.push_back(new ComponentRenderer(0, 0, 200, 0, style));
				compRenderers.back()->setComponent(components.back());
				view.add(*compRenderers.back());
			}
		}

		if (btnPlay.click() && (!playing || paused))
		{
			float volume = inputVolume.getFloat();
			signal.getSound()->setVolume(volume);
			signal.getSound()->play();
			playing = true;
			paused = false;
		}
		if(btnStop.click() && playing)
		{
			signal.getSound()->stop();
			playing = false;
		}
		if (btnPause.click() && (playing && !paused))
		{
			signal.getSound()->pause();
			paused = true;
		}

		if (btnSave.click())
		{
			FileManager::Save("test.json", outputRenderer, compRenderers);
		}
		if (btnLoad.click())
		{
			FileManager::Load("test.json", view, outputRenderer, compRenderers, components);
		}

		if (btnExport.click())
		{
			FileManager::ExportWav("test.wav", *signal.getSound()->getBuffer());
		}

		if (view.hovered(Input::GetMousePosition()))
		{
			float delta = Input::GetMouseWheelScroll();
			if (delta != 0)
			{
				view.setZoom(clamp(view.getZoom() * (1.0f - 0.05f * delta), maxZoomIn, maxZoomOut));
			}

			if (Input::GetMouseButton(sf::Mouse::Middle))
			{
				view.setCenter(view.getCenter() - view.mapScreenVectorToView(prevMousePos, Input::GetMousePosition()));
			}

			if (Input::GetMouseButtonDown(sf::Mouse::Right))
			{
				AbstractUI* hoveredUI = UIManager::GetFirstHoveredUI(Input::GetMousePosition());
				ComponentRenderer* hoveredComp = dynamic_cast<ComponentRenderer*>(hoveredUI);
				if (hoveredComp != selectedComp)
				{
					if (selectedComp != nullptr)
					{
						selectedComp->setFocused(false);
					}
					selectedComp = hoveredComp;
					if (selectedComp != nullptr)
					{
						selectedComp->setFocused(true);
					}
				}
				else if (selectedComp != nullptr)
				{
					selectedComp->setFocused(false);
					selectedComp = nullptr;
				}
			}
		}

		if (Input::GetKeyDown(sf::Keyboard::Delete))
		{
			if (selectedComp != nullptr)
			{
				vector<ComponentRenderer*>::iterator renderIt = find(compRenderers.begin(), compRenderers.end(), selectedComp);
				if (renderIt != compRenderers.end())
				{
					vector<Component*>::iterator it = find(components.begin(), components.end(), selectedComp->getComponent());
					if (it != components.end())
					{
						if (view.remove(**renderIt))
						{
							delete *renderIt;
							delete *it;
							compRenderers.erase(renderIt);
							components.erase(it);
							selectedComp = nullptr;
						}
					}
				}
			}
		}

		if (Input::GetMouseButtonDown(sf::Mouse::Left))
		{
			sf::Vector2f mousePos = Input::GetMousePosition();
			//cout << "Mouse position: (" << mousePos.x << "," << mousePos.y << ")" << endl;
		}

		// ///////////////////////// START DRAW
		Window::Clear();


		Window::Draw(); // Draw active layout (with all its children)

		// ///////////////////////// END DRAW
		Window::Display();
	}

	for (int i = 0; i < components.size(); i++)
	{
		delete components[i];
	}

	for (int i = 0; i < compRenderers.size(); i++)
	{
		delete compRenderers[i];
	}

	return 0;
}
