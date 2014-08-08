solution "ElTrazadoDeRayos"

language "C++"
location("./" .. _ACTION)  -- Where to put the project files.
includedirs {"../include", "../include/glm", "../include/rapidxml", "../src/RayTracerLib"}
libdirs {"../lib"}
flags {"StaticRuntime"}

-- Add the c++ 11 standard if we use gmake.
if (_ACTION == "gmake") then
  buildoptions {"-std=c++0x -std=gnu++0x"}
end

-- Define which OS we use.
if os.get() == "windows" then
  defines {"WINDOWS"}
elseif os.get() == "linux" then 
  defines {"LINUX"}
elseif os.get() == "macosx" then
  defines {"MACOSX"}
end

-- Configurations.
configurations {"Release", "Debug", "Profile"}

-- Platforms
platforms { "native", "universal" }
-- platforms {"x32", "x64"}

configuration "Release"
  defines {"NDEBUG", "PASTEL_ENABLE_OMP" }
  flags {"Optimize"}
  targetdir "../bin/release"

configuration { "gmake",  "release" }
    buildoptions { "-fopenmp" }
    links { "gomp" }

configuration "Debug"
  defines {"DEBUG"}
  flags {"Symbols"}
  targetdir "../bin/debug"
  
configuration { "gmake",  "Debug" }
  links { "gomp" }

configuration "Profile"
  defines {"PROF", "PASTEL_ENABLE_OMP"}
  flags {"Symbols", "Optimize"}
  targetdir "../bin/prof"
  
configuration { "gmake",  "Profile" }
  buildoptions{ "-pg", "-fopenmp" }
  links { "gomp" }
  linkoptions( "-pg" )

-- Enable openmp for visual studio project files.
configuration { "vs*",  "release" }
  buildoptions { "/openmp" }

-- Projects.
project "ElTrazadoDeRayosLib"
  files {"../src/RayTracerLib/**"}
  kind "StaticLib"
  if os.get() == "windows" then
    postbuildcommands { "py ..\\cpplintHelper.py --root=src ..\\..\\src\\RayTracerLib" }
  elseif os.get() == "linux" then 
    postbuildcommands { "-@python2 ../cpplintHelper.py --root=src ../../src/RayTracerLib > /dev/null" }
  end

  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("f2a30267-2beb-426c-9fc1-cc24c4ba9d21")

project "ConsoleMain"
  files {"../src/ConsoleMain/**"}
  kind "ConsoleApp"
  links {"ElTrazadoDeRayosLib"}
  if os.get() == "windows" then
    postbuildcommands { "py ..\\cpplintHelper.py --root=src ..\\..\\src\\ConsoleMain" }
  elseif os.get() == "linux" then 
    postbuildcommands { "-@python2 ../cpplintHelper.py --root=src ../../src/ConsoleMain > /dev/null" }
  end
  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("865dfdb0-97ae-4dce-b70f-9d4a11413162")

