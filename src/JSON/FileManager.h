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

using namespace std;

class FileManager
{
private:
public:
	FileManager();
	~FileManager();



	bool ToJSON(const vector<class ComponentRenderer*>& _inComponentRenderer, string& _outJson);
	bool FromJSON(const string& _inJson, vector<class ComponentRenderer*>& _outComponentRenderers, vector<class Component*>& _outComponents);

};

#endif // _FILEMANAGER_H