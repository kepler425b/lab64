@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 
mkdir ..\build
pushd ..\build

cl -Zi ..\code\game_main.cpp ..\code\imgui_impl_sdl.cpp ..\code\imgui_impl_opengl3.cpp ..\code\imgui.cpp ..\code\imgui_demo.cpp  ..\code\imgui_draw.cpp -W1 -EHsc -I ..\include -I ..\dependencies\glm -I ..\dependencies\glew-2.1.0\include user32.lib kernel32.lib winmm.lib dsound.lib gdi32.lib opengl32.lib glu32.lib glew32s.lib SDL2_ttf.lib glfw3.lib freetype.lib SDL2.lib SDL2main.lib SDL2_image.lib -link -LIBPATH:..\lib -LIBPATH:..\dependencies\glew-2.1.0\lib\Release\Win32\ -SUBSYSTEM:CONSOLE -MACHINE:x86 -OUT:..\code\b.exe

IF %ERRORLEVEL% EQU 0 (
	goto good 
) 

IF %ERRORLEVEL% NEQ 0 (
	goto bad
)

:good
	popd  
	echo --COMPILATION OK--
	call ..\code\b.exe 
	goto end

:bad 
	echo --COMPILATION FAILED--
	goto end

:end
