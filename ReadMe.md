[link_projects]: https://github.com/BenPyton/SoundGenerator/projects/1
[link_issues]: https://github.com/BenPyton/SoundGenerator/issues
[link_pullRequest]: https://github.com/BenPyton/SoundGenerator/pulls

# Sound Generator with Nodal Interface

I don't have a good name at this time, but this software will be renamed later.

<img src="Images/screen01.jpg?raw=true" width="800"></img>

## Description

This software is a nodal editor that allow you to create sounds from several generators (sine wave, square wave, etc.) 
and modulate them (frequency, amplitude, etc.) and modify them (delay, repeat, etc.).

### Warnings
**This is currently in alpha version.**<br>
It still lack of features and components: there a lot more to be done.<br>
But the software is fully functionnal and have all the core features.<br>
See the [roadmap to beta version][link_projects] to get an idea of what is still to be done. 

**There is currently only Windows 8/10 builds**<br>
If you want this software on other platform, please download Qt and build it yourself (see the [Technical Informations](#technical-informations) section to get Qt).<br>
You can also contribute by providing your build to share it with other people.

## Controls
Press any buttons from the left side to add a component in the scene.<br>
You can also drag and drop from one of those buttons to add a component at a position.<br>
Or you can `right click > add` in the scene to add a component at a position.

You can select components with `mouse left button` and drag them around the scene.<br>
You can delete selected components with `right click > remove` or by pressing `delete` key.<br>
(note that the output component cannot be removed)<br>

Press a component pin with `left mouse button` and hold it until you release it on another pin.<br>
An output pin cannot be connected to another output pin (same for input pins), and you cannot connect components in a loop.<br>

In order to play a sound, you have to generate it with the menu `Audio > Generate` or by pressing `F5`.<br>
You can auto-generate the sound when play button is pressed, or when a modification is made, in the audio menu.

Common shortcuts:
- Ctrl+A: select all the components;
- Ctrl+C/Ctrl+V: copy/paste components (except the output);
- Ctrl+Z/Ctrl+Y: undo/redo your modifications.

## Features
- Base sound signals : Sinus, Square, Triangle, Sawtooth, Random (White noise);
- Mix sound signals with operators like Add, Multiply;
- Modify sound signals with Delay, Repeat and ADSR;
- Save and load projects;
- Save resulting sound in wav file.

Visit the [planned features][link_projects] to see what are the next features to come.<br>
**Warning**: this project is made on my freetime, so it can take a while before releasing a new version.

If you think of another feature, or you found bugs, please go to the [issue panel][link_issues] to report it, or fix it yourself and do a [Pull Request][link_pullRequest] to contribute to the software.<br>
(Be careful of the licenses from Qt and this software if you modify it for your own purpose)

## Technical informations
This project is made with Qt 5.12.2 and QtCreator 4.8.2.<br>
You can get Qt from [here](http://download.qt.io/official_releases/qt/5.12/5.12.2/)<br>
And if you want to use QtCreator, get it from [here](http://download.qt.io/official_releases/qtcreator/4.8/4.8.2/).

You can use another version of Qt or QtCreator but I cannot guarranty that it can compile or the software runs correctly.

## License
<img src="https://www.gnu.org/graphics/gplv3-with-text-136x68.png"/><br>
This project is under GNU General Public License (GPLv3) version 3.<br>
Copyright (c) 2019-2020 Benoît Pelletier<br>
Please refer to this software license text [here](LICENSE), and third party attributions [here](attributions.md).
