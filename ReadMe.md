# Sound Generator with Nodal Interface

<b>Author :</b> PELLETIER Benoît

<img src="images/screen01.png?raw=true" width="800"></img>

### Description :
Nodal editor to create sounds from scratch. <br>

Visual Studio 2015 or newer is required to run this project correctly. <br>
Please install one or both of VC++15 SFML from [here](https://www.sfml-dev.org/download/sfml/2.5.1/index-fr.php) at:
- 32 bits: "$(SolutionDir)/../SFML-2.5.1"
- 64 bits: "$(SolutionDir)/../SFML-2.5.1-x64"

If you have issues with compilation, check the version of windows' SDK in project properties. <br>

### Features :
\[Done\]
- Base sound signals : Sinus, Square, Triangle, Sawtooth, Random (White noise);
- Mix sound signals with operators like Add, Multiply;
- Modify sound signals with Delay, Repeat and ADSR;
- Save and load editor projects;
- Save resulting sound in wav file.

\[Planned\]
- Switch to Qt interface;
- Add more components.
