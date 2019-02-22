/*
* @author PELLETIER Benoit
*
* @file FileManager.h
*
* @date 18/02/2019
*
* @brief Manage save files with json
*
*/

#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>

using namespace std;

class ComponentRenderer;
class Component;
class View;

class FileManager
{
private:
	FileManager();
public:
	~FileManager();

	static bool ExportWav(string _fileName, const sf::SoundBuffer& _sound);

	static bool SaveConfig(string _fileName);
	static bool LoadConfig(string _fileName);

	static bool Save(string _fileName, ComponentRenderer& _inOutput, const vector<ComponentRenderer*>& _inComponentRenderer);
	static bool Load(string _fileName, View& _view, ComponentRenderer& _outOutput, vector<ComponentRenderer*>& _outComponentRenderer, vector<Component*>& _outComponents);


	static bool ToJSON(ComponentRenderer& _inOutput, const vector<ComponentRenderer*>& _inComponentRenderer, string& _outJson);
	static bool FromJSON(const string& _inJson, View& _view, ComponentRenderer& _outOutput, vector<ComponentRenderer*>& _outComponentRenderers, vector<Component*>& _outComponents);

};

#endif // _FILEMANAGER_H