#!lua

-- https://github.com/premake/premake-4.x/wiki/Scripting-With-Premake

solution "crossword-editor" configurations { "Debug", "Release" }
  --buildoptions { "-std=c++11" }
  defines { "GCC_C_LANGUAGE_STANDARD=c11" }

  configuration "Debug"
   defines { "DEBUG" }
   flags { "Symbols", "ExtraWarnings" }

  configuration "Release"
   defines { "NDEBUG" }
   flags { "Optimize" }

  project "crossword-editor"
  	language "C"
  	kind "ConsoleApp"
  	files { "editor.c" }
  	targetname ("editor")
  	links { "ncurses" }

