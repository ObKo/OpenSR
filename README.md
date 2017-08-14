# How to build

## Prerequisites

* cmake >= 3.0
* Qt >= 5.9
* OpenAL
* mpg123

## Building

    mkdir build
    cd build
    cmake -DBUILD_ALL_TOOLS=Yes -DCMAKE_BUILD_TYPE=Debug ../
    make 
    
# How to run

First of all, you'll need resources from original game. OpenSR now can only use 
"Space Rangers 2: Reboot" ("Космические Рейнджеры 2: Доминаторы. Перезагрузка")
release of game. 


Copy following .pkg files (game resources) from install ISO to data/ folder:
    
    SR2/DATA/2captain.pkg
    SR2/DATA/2gov.pkg
    SR2/DATA/2Items.pkg
    SR2/DATA/2main.pkg
    SR2/DATA/common.pkg
    SR2/DATA/ShipFull.pkg
    SR2/DATA/ShipSmall.pkg
    SR2/DATA/Sound.pkg
    SR2/DATA/Star.pkg

Copy music from install ISO to data/ folder:

    SR2/Music/

*All the following commands assumed to be run from project root dir with `build` as build directory*

Next you have to decode several .dat files using previously built DATTools 
(tools/DATTools):

    ./build/tools/DATTools/opensr-dat-convert hd <iso>/SR2/CFG/CacheData.dat data/CacheData.dat
    ./build/tools/DATTools/opensr-dat-convert d <iso>/SR2/CFG/Main.dat data/Main.dat
    ./build/tools/DATTools/opensr-dat-convert d <iso>/SR2/CFG/Rus.dat data/Rus.dat

(You can replace `SR2/CFG/Rus.dat` with `SR2/CFG/Eng.dat` for English version)

Also, OpenSR requires that some libraries should be available in working directory:
    
    ln -s build/World/libworld.so
    mkdir imageformats
    ln -s ../build/ImagePlugin/libQtOpenSRImagePlugin.so imageformats/libQtOpenSRImagePlugin.so

Finally, run OpenSR:
    
    ./build/Engine/opensr

