/*
 * Copyright 2019 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <QString>

class ComponentFactory
{
private:
	ComponentFactory() {}

public:
	// Create a component from it's name
	// Return nullptr if _compName doesn't corresponding to any component name
    static class Component* CreateComponent(const QString& _compName);
};

#endif // COMPONENTFACTORY_H