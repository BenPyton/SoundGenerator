# Automated Installer Generation

This automated installer generation create installers for Windows 8/10 32 bits and 64 bits for the software built with Qt Microsoft Visual C++ 2015.

To create an installer, you need first to install [Inno Setup](http://www.jrsoftware.org/isdl.php) and [Qt 5.12.2 MSVC 32 bits and/or 64 bits](http://download.qt.io/official_releases/qt/5.12/5.12.2/).

Then add an environment variable called QT_PATH like "C:\Qt\Qt 5.12.2\5.12.2",<br>
and a path variable to Inno Setup directory like "C:\ProgramFiles (x86)\Inno Setup 6".

Copy the executable files of the software built by Qt into the corresponding x64 or x86 directories.

Finally, run batch files corresponding to the Qt architectures you've installed (x86 for 32 bits and x64 for 64 bits).
