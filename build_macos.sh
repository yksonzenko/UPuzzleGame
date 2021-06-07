# Build for Xcode compiler. 

cmake -G "Xcode" -Bbuild -DCMAKE_CONFIGURATION_TYPES="Debug;Release"

cmake --build ./build
