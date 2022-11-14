echo "Remove and create build folder"
rm -r -f build
mkdir build
cd build

echo "Configure cmake"
emcmake cmake ..