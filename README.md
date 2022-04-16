# Team 37

## Target Environment

Item | Version
-|-
OS | Windows 10
Toolchain | Microsoft Visual Studio Enterprise 2019 Version 16.11.0
C++ Standard | C++17

## Additional Build Instructions

### For Windows Environment
Build the project by following the instructions mentioned in [project-wiki](https://github.com/nus-cs3203/project-wiki/wiki/Tools-Cross-platform-Startup-SPA-Solution#22-working-with-cmake-in-visual-studio).

To run system testing, please
1. Build the project
2. Navigate to `21s2-cp-spa-team-37/Team37/Tests37`
3. Create a new folder `out` to store all output `.xml` files
4. Run `win_test_all.ps1` in power shell, the output `.xml` files will be stored in `21s2-cp-spa-team-37/Team37/Tests37/out` folder


### For Mac Environment 
##### (Assume you have properly installed cmake, if not, `brew install cmake llvm`)
For auto evaluate can run the shell script named "runAutotesterMac" under: /runAutotester 

0. Manually run the build evaluate:
1. Navigate to Code37 Folder
2. mkdir build (If there is already one, remove it)
3. cd build
4. cmake ..
5. make -j4
6. cd ..
7. ./build/src/autotester/autotester ../Tests37/Sample_source.txt ../Tests37/Sample_queries.txt ../Tests37/out.xml

# Team Members

Name | Mobile | Email | Development OS/Toolchain
-:|:-:|:-|-|
Chia Jia-Xi, Kymie | 81634678 | e0418141@u.nus.edu | Windows 10/Visual Studio 2019
XIE YAOREN | 91702689 | xie.yaoren@u.nus.edu | MacOS/CLion
Jin Yuze | 93751694 | e0376926@u.nus.edu | MacOS/Clion
Tan Jia Qi | 91656092 | e0407759@u.nus.edu | Windows 10/Visual Studio 2019
Wayne Tan Kin Lun | 88176618 | e0407782@u.nus.edu | Windows 10/Visual Studio 2019
Chi Xu | 89428630 | e0589913@u.nus.edu | Windows 10/Visual Studio 2019
