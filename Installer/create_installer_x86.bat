@echo ===== Deploying Qt libraries =====
%QT_PATH%\msvc2017\bin\windeployqt.exe --release ./x86/
@echo ===== Creating setup wizard =====
@set /p VERSION=<version.txt
iscc "/DBuildDir=%CD%" "/DMyAppVersion=%VERSION%" soundgenerator_installer.iss
@echo ===== Generate Hash =====
@SET PowerShellScriptPath=%~dp0create_hash.ps1
@SET InputFile=installs/soundgenerator_%VERSION%_x86.exe
@SET OutputFile=installs/sha2.x86.txt
@PowerShell -NoProfile -ExecutionPolicy Bypass -Command "& '%PowerShellScriptPath%' '%InputFile%' -output '%OutputFile%'";
@echo ===== End =====
@pause