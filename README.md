ElTrazadoDeRayos
================
## Current progress ##
### Wed Jul 30 20:40:04 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/CornellRoomCheckerBoard.png "Cornell Room with blue|white CheckerBoard floor.")

<!--
### Thu July 17 14:02:45 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/CornellRoom.png "Monte Carlo")

### Wed June 02 14:02:22 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/CSG_anim.gif "CSG: :cool: stuff")

### Wed May 21 18:14:52 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/SimulatedSmoothShadow.gif "Newest Image.")

### Thu May 15 18:05:16 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/Anim.gif "Glass material in the middle.")

### Mon May 12 13:55:01 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/PerspectivePhongWithShadow.gif "A small animation of 20 rendered Frames with Phong shading and shadows")

### Sun May 11 17:42:24 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/PerspectivePhong.gif "A small animation of 20 rendered Frames with Phong shading")

### Fri May  9 13:42:55 CEST 2014 ###
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/OrthoAnim.gif "A small animation of 20 rendered Frames.")

### Wed May  7 22:24:19 CEST 2014 ######
![alt text](https://github.com/CantTouchDis/ElTrazadoDeRayos/raw/master/images/Ortho.bmp "First Trace")
-->


## How to build ##
This project uses premake to generate project files for either Linux, Windows or MacOS compilers. For more information on premake, visit the [premake-homepage](http://industriousone.com/premake).

Furthermore, to maintain a clean code style, _cpplint.py_ by google (see [google-styleguide](https://code.google.com/p/google-styleguide/)) is used. It will be automatically executed when building the project. Please make sure you have python 2.x installed.

### Windows ###
This project uses some C++11 standards and thus needs at least Visual Studio 2013. As premake does currently not support VS2013-Solution-files, you need to create solution files for Visual Studio 2012 and manually upgrade them to VS2013.

1. Navigate to the folder _./projects/_
2.  Execute ```premake4b5_windows.exe vs2012```. For convenience you can also just run the ```BuildWindowsProjects.bat``` script, that first cleans the current vs2012 solution folder and then builds it again.
3. Open the newly created _./projects/vs2012/_ folder. There you will find a _EltrazadaDeRayos.sln_ solution file. Open it with Visual Studio 2013.
4. Right click on the solution in the solution explorer in Visual Studio. Select _"Retarget Solution"_.
5. Build the solution. The final executable will be placed in _./bin/release/_ or _./bin/debug/_ respectively.


### Linux ###
Under Linux, this project is only tested with gcc. In theory it should also work with clang. As it needs some new C++11 standards, you need at least version 4.8 of gcc.

1. Navigate to the folder _./projects/_
2. Execute ```premake4b5_linux gmake```. For convenience you can also just run the ```BuildLinuxProjects.sh``` script, that first cleans the current gmake solution folder and then builds it again.
3. Navigate to the newly created _./projects/gmake/_ folder.
4. Run ```make```. The final executable will be placed in _./bin/release/_ or _./bin/debug/_ respectively.

### MacOS ###
Compiling the project under MacOS is not tested. However it should probably work analogously to Linux.


## License ##
All source code files are licensed under the MIT license (see LICENSE). The license files for _cpplint.py_, the _glm_ library and _premake_ can be found in the _./licenses/_ subfolder.
