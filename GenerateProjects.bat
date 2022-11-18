@echo off
set /p "id=Which version of Visual Studio do you want to use? (vs2019 or vs2022): "
premake5 %id%
pause