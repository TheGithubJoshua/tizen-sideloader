#### Sideload apps on Samsung Tizen TVs without messing around with Tizen Studio.

![](https://files.rickroll.space/content/cdn/eiqDEZeCFill.png) 

*Beautiful ImGui UI*

# How to Build
(Only tested on Linux, support of other Operating Systems is not guaranteed.)

## Windows with Visual Studio's CLI

Use build_win32.bat or directly:
```
set SDL2_DIR=path_to_your_sdl2_folder
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x86 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
#                 ^^ include paths                  ^^ source files                                                                                    ^^ output exe                    ^^ output dir   ^^ libraries
# or for 64-bit:
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x64 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
```

## Linux and similar Unixes

Use our Makefile or directly:
```
c++ `sdl2-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl2-config --libs` -lGL -ldl
```

## macOS

Use our Makefile or directly:
```
brew install sdl2
c++ `sdl2-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl2-config --libs` -framework OpenGl -framework CoreFoundation
```