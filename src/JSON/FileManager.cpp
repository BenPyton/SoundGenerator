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

// https://github.com/nlohmann/json

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

bool FileManager::ToJSON(const vector<class ComponentRenderer*>& _inComponentRenderer, string & _outJson)
{
	return false;
}

bool FileManager::FromJSON(const string & _inJson, vector<class ComponentRenderer*>& _outComponentRenderers, vector<class Component*>& _outComponents)
{
	return false;
}
