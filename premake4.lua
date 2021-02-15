#!lua

-- https://github.com/premake/premake-4.x/wiki/Scripting-With-Premake

solution "crossword-editor" configurations { "debug", "debugnocurses", "release" }
  --buildoptions { "-std=c++11" }
  defines { "GCC_C_LANGUAGE_STANDARD=c11" }

  --cleancommands { "rm -rf editor obj data.bin" }

  configuration "debug"
    defines { "DEBUG", "W_CURSES" }
    flags { "Symbols", "ExtraWarnings" }
  
  configuration "debugnocurses"
    defines { "DEBUG" }
    flags { "Symbols" }

  configuration "release"
    defines { "W_CURSES" }
    flags { "Optimize" }

  project "crossword-editor"
  	language "C"
  	kind "ConsoleApp"
  	files { "src/editor.c" }
  	targetname ("editor")
  	links { "ncurses" }

