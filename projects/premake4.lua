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
if os.get() == "windows" then
  defines {"WINDOWS"}
elseif os.get() == "linux" then 
  defines {"LINUX"}
elseif os.get() == "macosx" then
  defines {"MACOSX"}
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
if os.get() == "windows" then
  -- There is sadly no wildcard for subfolders in Windows, thus we need
  -- to specify these manually.
  -- TODO(cgissler, 05/02/2014): This should probably be in the respective
  -- projects so only the project that is build is linted (and only when it's
  -- debug-build).
  -- prebuildcommands { "py ../cpplint.py ..\\..\\src\\RayTracerLib\\*.h" }
  -- prebuildcommands { "py ../cpplint.py ..\\..\\src\\ConsoleMain\\*" }
  -- prebuildcommands { "dir ..\\..\\src\\RayTracerLib" }
  -- TODO(cgissler, 05/02/2014): Add back the -v2 when I sorted out the
  -- install issues with python3 and python2 on my pc.
  --prebuildcommands { "py -v2 ./cpplint.py ../src/RayTracerLib/*" }
  --prebuildcommands { "py -v2 ./cpplint.py ../src/ConsoleMain/*" }
elseif os.get() == "linux" then 
  --postbuildcommands { 'python ../cpplint.py --root=src %{cfg.buildtarget.abspath}' }
  --prebuildcommands { "python ../cpplint.py --root=src ../../src/ConsoleMain/* &2>1" }
elseif os.get() == "macosx" then
  --prebuildcommands { "enter command here" }
end

-- Projects.
project "ElTrazadoDeRayosLib"
  files {"../src/RayTracerLib/**"}
  kind "StaticLib"
  if os.get() == "linux" then 
    postbuildcommands { "python ../cpplint.py --root=src ../../src/RayTracerLib/** &2>1" }
  end

  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("f2a30267-2beb-426c-9fc1-cc24c4ba9d21")

project "ConsoleMain"
  files {"../src/ConsoleMain/**"}
  kind "ConsoleApp"
  links {"ElTrazadoDeRayosLib"}
  if os.get() == "linux" then 
    postbuildcommands { "python ../cpplint.py --root=src ../../src/ConsoleMain/** &2>1" }
  end
  -- This is nice to have so VS always uses the same uuids in its project files.
  -- Generated via http://www.uuidgenerator.net
  uuid("865dfdb0-97ae-4dce-b70f-9d4a11413162")

