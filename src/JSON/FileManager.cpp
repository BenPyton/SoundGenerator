/*
* @author PELLETIER Benoit
*
* @file FileManager.cpp
*
* @date 18/02/2019
*
* @brief Manage save files with json
*
*/

#include "stdafx.h"
#include "FileManager.h"
#include "json.hpp" // Copyright nlohmann / 2013-2018
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include "../Components/Component.h"
#include "../Components/ComponentRenderer.h"
#include "../Components/ComponentFactory.h"
#include "../Core/View.h"

// https://github.com/nlohmann/json

#define VERSION "0.0.1"

using json = nlohmann::json;

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

bool FileManager::ExportWav(string _fileName, const sf::SoundBuffer& _sound)
{
	/*bool success = false;
	sf::OutputSoundFile outputFile;
	if (!outputFile.openFromFile(_fileName, _sound.getSampleRate(), _sound.getChannelCount()))
	{
		cerr << "Error when opening output file." << endl;
	}
	else
	{
		outputFile.write(_sound.getSamples(), _sound.getSampleCount());
		success = true;
	}
	return success;*/

	return _sound.saveToFile(_fileName);
}

bool FileManager::SaveConfig(string _fileName)
{
	return false;
}

bool FileManager::LoadConfig(string _fileName)
{
	return false;
}

bool FileManager::Save(string _fileName, ComponentRenderer& _inOutput, const vector<ComponentRenderer*>& _inComponentRenderer)
{
	ofstream outFile(_fileName);
	if (outFile.is_open())
	{
		string str;
		ToJSON(_inOutput, _inComponentRenderer, str);
		outFile << str;
	}
	else
	{
		cout << "Error when saving into file: " << _fileName << endl;
	}

	outFile.close();

	return false;
}

bool FileManager::Load(string _fileName, View& _view, ComponentRenderer& _outOutput, vector<ComponentRenderer*>& _outComponentRenderer, vector<Component*>& _outComponents)
{
	ifstream inFile(_fileName);
	if (inFile.is_open())
	{
		string str((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
		//cout << str << endl;
		FromJSON(str, _view, _outOutput, _outComponentRenderer, _outComponents);
	}
	else
	{
		cout << "Error when loading from file: " << _fileName << endl;
	}

	inFile.close();


	return false;
}

int GetId(const vector<class ComponentRenderer*>& _compList, const Component* _comp)
{
	int id = -1;
	if (_comp != nullptr)
	{
		for (int i = 0; i < _compList.size(); i++)
		{
			if (_comp == _compList[i]->getComponent())
			{
				id = i;
				break;
			}
		}
	}
	return id;
}

bool FileManager::ToJSON(ComponentRenderer& _inOutput, const vector<ComponentRenderer*>& _inComponentRenderer, string & _outFile)
{
	json outJson;

	outJson["version"] = VERSION;
	outJson["components"] = json::array();

	outJson["output"]["x"] = _inOutput.getPosition().x;
	outJson["output"]["y"] = _inOutput.getPosition().y;
	outJson["output"]["connection"] = GetId(_inComponentRenderer, _inOutput.getComponent()->getInput(0)->getComponent());

	for (int i = 0; i < _inComponentRenderer.size(); i++)
	{
		Component* comp = _inComponentRenderer[i]->getComponent();

		json compJson;
		compJson["id"] = i;
		compJson["x"] = _inComponentRenderer[i]->getPosition().x;
		compJson["y"] = _inComponentRenderer[i]->getPosition().y;
		compJson["name"] = comp->getName();


		compJson["inputs"] = json::array();
		for (int k = 0; k < comp->getInputCount(); k++)
		{
			json compInputJson;
			compInputJson["name"] = comp->getInput(k)->getName();
			compInputJson["connection"] = GetId(_inComponentRenderer, comp->getInput(k)->getComponent());
			compInputJson["default value"] = comp->getInput(k)->getDefaultValue();
			compJson["inputs"].push_back(compInputJson);
		}

		outJson["components"].push_back(compJson);
	}

	_outFile = outJson.dump(1, '\t');

	return false;
}

bool FileManager::FromJSON(const string & _inFile, View& _view, ComponentRenderer& _outOutput, vector<ComponentRenderer*>& _outComponentRenderers, vector<Component*>& _outComponents)
{
	json inJson = json::parse(_inFile);

	if (VERSION == inJson["version"])
	{
		// Delete all existing components
		for (ComponentRenderer* compRender : _outComponentRenderers)
		{
			_view.remove(*compRender);
			delete compRender;
		}
		_outComponentRenderers.clear();

		for (Component* comp : _outComponents)
		{
			delete comp;
		}
		_outComponents.clear();

		// first loop to load all components
		for (int i = 0; i < inJson["components"].size(); i++)
		{
			json comp = inJson["components"][i];
			_outComponents.push_back(ComponentFactory::CreateComponent(comp["name"].get<string>())); 
			for (int k = 0; k < comp["inputs"].size(); k++)
			{
				_outComponents.back()->getInput(comp["inputs"][k]["name"].get<string>())->setDefaultValue(comp["inputs"][k]["default value"].get<float>());
			}
			_outComponentRenderers.push_back(new ComponentRenderer(0, 0, 200, 0, *_outOutput.getStyle()));
			_outComponentRenderers.back()->setComponent(_outComponents.back());
			_outComponentRenderers.back()->setPosition(sf::Vector2f(comp["x"].get<float>(), comp["y"].get<float>()));
			_view.add(*_outComponentRenderers.back());
		}

		// second loop to connect them
		for (int i = 0; i < _outComponentRenderers.size(); i++)
		{
			for (int k = 0; k < inJson["components"][i]["inputs"].size(); k++)
			{
				json input = inJson["components"][i]["inputs"][k];

				//_outComponentRenderers[i]->getInput(input["name"].get<string>())->getInput()->setDefaultValue(input["default value"]);

				int conn = input["connection"].get<int>();
				if (conn >= 0)
				{
					_outComponentRenderers[i]
						->getInput(input["name"].get<string>())
						->connect(_outComponentRenderers[conn]->getOutput());
				}

			}
		}

		// Set output position and connection
		_outOutput.setPosition(sf::Vector2f(inJson["output"]["x"], inJson["output"]["y"]));
		int conn = inJson["output"]["connection"].get<int>();
		if (conn >= 0)
		{
			_outOutput.getInput("")->connect(_outComponentRenderers[conn]->getOutput());
		}
	}
	else
	{
		cout << "Error when loading file: wrong version of json file." << endl;
	}

	return false;
}
