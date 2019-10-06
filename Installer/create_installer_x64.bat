@echo ===== Deploying Qt libraries =====
%QT_PATH%\msvc2017_64\bin\windeployqt.exe --release ./x64/
@echo ===== Creating setup wizard =====
iscc /Dx64 "/DBuildDir=%CD%" soundgenerator_installer.iss
@echo ===== Generate Hash =====
@SET PowerShellScriptPath=%~dp0create_hash.ps1
@SET InputFile=installs/soundgenerator_0.0.1_x64.exe
@SET OutputFile=installs/sha2.x64.txt
@PowerShell -NoProfile -ExecutionPolicy Bypass -Command "& '%PowerShellScriptPath%' '%InputFile%' -output '%OutputFile%'";
@echo ===== End =====
@pause