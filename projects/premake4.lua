solution "ElTrazadoDeRayos"

language "C++"
location("./" .. _ACTION)  -- Where to put the project files.
includedirs {"../include", "../include/glm", "../src/**"}
libdirs {"../lib"}
flags {"StaticRuntime"}

-- Add the c++ 11 standard if we use gmake.
if (_ACTION == "gmake") then
  buildoptions {"-std=c++0x -std=gnu++0x"}
end

-- Define which OS we use.
-- The linter is currently not used, because it does not support python3.x
if os.get() == "windows" then
  defines {"WINDOWS"}
  --postbuildcommand { "python ./cpplint.py
elseif os.get() == "linux" then 
  defines {"LINUX"}
  --postbuildcommand { "python ./cpplint.py
elseif os.get() == "macosx" then
  defines {"MACOSX"}
  --postbuildcommand { "python ./cpplint.py
end

-- Configurations.
configurations {"Release", "Debug"}

-- Platforms
platforms { "native", "universal" }
-- platforms {"x32", "x64"}

configuration "Release"
  defines {"NDEBUG"}
  flags {"Optimize"}
  targetdir "../bin/release"

configuration "Debug"
  defines {"DEBUG"}
  flags {"Symbols"}
  targetdir "../bin/debug"

-- Projects.
project "ElTrazadoDeRayosLib"
  files {"../src/RayTracerLib/**"}
  kind "StaticLib"
  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("f2a30267-2beb-426c-9fc1-cc24c4ba9d21")

project "ConsoleMain"
  files {"../src/ConsoleMain/**"}
  kind "ConsoleApp"
  links {"ElTravadoDeRayosLib"}
  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("865dfdb0-97ae-4dce-b70f-9d4a11413162")
