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
#include "Components/SinusComponent.h"
#include "Components/SquareComponent.h"

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

	sf::Text fpsText("FPS: ??", font, 16);
	fpsText.setPosition(10, 10);
	fpsText.setFillColor(lightGrey);

	UIStyle style;
	style.setFont(font, 16);
	style.mapStyle(UI_NORMAL, lightGrey, sf::Color::Transparent, lightGrey, 1.0f);
	style.mapStyle(UI_HOVERED, lightGrey, sf::Color(50, 50, 50), lightGrey, 1.0f);
	style.mapStyle(UI_CLICKED, sf::Color::Black, lightGrey, lightGrey, 1.0f);
	style.mapStyle(UI_DISABLED, lightGrey, sf::Color::Red, lightGrey, 1.0f);
	style.mapStyle(UI_FOCUSED, lightGrey, sf::Color::Blue, lightGrey, 1.0f);


	// Button Example
	Button button(10, -10, 180, 30, style);
	button.setText("Generate");
	button.setMargins(10, 10, 10, 10);
	button.setAnchorMin(sf::Vector2f(0.0f, 1.0f));
	button.setAnchorMax(sf::Vector2f(0.0f, 1.0f));
	button.setPivot(sf::Vector2f(0.0f, 1.0f));


	// InputField Example
	InputField input(10, 50, 150, 30, style);
	//input.setFont(font);
	//input.setCharacterSize(16);
	input.setMaxLength(10);
	input.setFloat(1.0f);
	input.setPlaceholder("Test");

	// Draggable box example
	DraggableBox box(100, 100, 200, 100, style);



	Button button1(-10, 10, 150, 30, style);
	button1.setText("1");

	Button button2(10, 10, 150, 30, style);
	button2.setText("2");

	Button button3(-10, 10, 150, 30, style);
	button3.setText("3");

	Button button4(-10, 10, 150, 30, style);
	button4.setText("4");

	VerticalLayout vLayout(0, 100, 200, 200);
	vLayout.setAnchorMin(sf::Vector2f(0, 0));
	vLayout.setAnchorMax(sf::Vector2f(0.0f, 1));
	vLayout.setMargins(10, 10, 100, 10);
	vLayout.setSpacing(10);
	vLayout.setPaddings(10, 10, 10, 10);
	vLayout.add(button1);
	vLayout.add(button2);
	vLayout.add(button3);
	vLayout.add(button4);

	// View
	View view(0, 0, 0, 0, style);
	view.setAnchorMin(sf::Vector2f(0, 0));
	view.setAnchorMax(sf::Vector2f(1, 1));
	view.setMargins(200, 10, 100, 10);
	view.add(box);

	// Window layout
	Layout rootLayout(0, 0, Window::GetWidth(), Window::GetHeight());
	Window::SetLayout(rootLayout);
	rootLayout.add(button);
	rootLayout.add(input);
	rootLayout.add(vLayout);
	rootLayout.add(view);




	/*sf::SoundBuffer buffer;
	sf::Sound sound(buffer);*/

	vector<sf::Int16> samples;
	/*sf::VertexArray vertices;
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);*/

	Signal signal;
	SignalRenderer signalRenderer(10, 10, 100, 80, style);
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

	SinusComponent sinusGenerator;
	sinusGenerator.getInput("Frequency")->setDefaultValue(20);

	SinusComponent sinusAttenuation;
	sinusAttenuation.getInput("Frequency")->setDefaultValue(1.5f);
	sinusAttenuation.getInput("Offset")->setDefaultValue(0.5f);
	sinusAttenuation.getInput("Amplitude")->setDefaultValue(0.5f);

	sinusGenerator.getInput("Amplitude")->setComponent(&sinusAttenuation);


	SquareComponent squareGenerator;
	squareGenerator.getInput("Frequency")->setDefaultValue(10);
	squareGenerator.getInput("Amplitude")->setDefaultValue(0.5f);
	squareGenerator.getInput("Offset")->setDefaultValue(0.5f);

	sinusGenerator.getInput("Amplitude")->setComponent(&squareGenerator);
	squareGenerator.getInput("Amplitude")->setComponent(&sinusGenerator);

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

		fpsText.setString("FPS: " + floatToStr(Time::GetFps(), 0));

		if (button.click())
		{
			samples.clear();
			//vertices.clear();

			float duration = input.getFloat();

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

				samples.push_back(0x7FFF * clamp(sinusGenerator.getOutput(x), -1.0f, 1.0f));
				//vertices.append(sf::Vertex(sf::Vector2f(i * step, 0.5f * Window::GetWidth() + mapValue(samples[i], -512, 512, -200, 200)), sf::Color::White));
			}
			//buffer.loadFromSamples(samples.data(), SAMPLERATE, 1, SAMPLERATE);
			signal.setData(samples);
			cout << "Signal sample count: " << signal.getSampleCount() << endl;
		}

		if (view.hovered(Input::GetMousePosition()))
		{
			float delta = Input::GetMouseWheelScroll();
			if (delta != 0)
			{
				view.setZoom(view.getZoom() + 0.05f * delta);
			}

			if (Input::GetMouseButton(sf::Mouse::Middle))
			{
				view.setCenter(view.getCenter() - view.mapScreenVectorToView(prevMousePos, Input::GetMousePosition()));
			}
		}

		if (Input::GetMouseButtonDown(sf::Mouse::Left))
		{
			sf::Vector2f mousePos = Input::GetMousePosition();
			cout << "Mouse position: (" << mousePos.x << "," << mousePos.y << ")" << endl;
		}

		// ///////////////////////// START DRAW
		Window::Clear();

		Window::Draw(fpsText);
		//Window::Draw(vertices);

		Window::Draw(); // Draw active layout (with all its children)

		// ///////////////////////// END DRAW
		Window::Display();
	}

	return 0;
}
