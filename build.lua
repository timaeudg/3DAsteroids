#!/usr/bin/env lua

local exec = os.execute;

-- Configuration Options
local cc = "g++ "
local cflags = "-c -Wall -std=c++98 "
local lflags = "-lsfml-graphics -lsfml-window -lsfml-system -lGL -lglut "
local srcdir = "./src/"
local builddir = "./obj/"
local sources = {"main.cpp", "Cube.cpp", "Cubes.cpp" }
local projname = "3DAsteroids"

-- Compile each source file to an appropriate object file
for _,file in pairs(sources) do
	io.write( "Compilling "..file.."... " )
	io.write(exec(cc..cflags..srcdir..file.." -o "..builddir..file:gsub("%.cpp",".o")) == 0 and "\t\tOK" or "\t\tFAILED!")
	sources[_] = builddir..file:gsub("%.cpp", ".o")
	io.write "\n"
end 

-- Link the object files to the final executeable
io.write("Linking "..projname.."... ")
io.write(exec(cc..lflags..table.concat(sources," ").." -o "..projname) == 0 and "\t\tOK" or "\t\tFAILED!")
io.write "\n"

