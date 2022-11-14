echo "Go to build"
cd build

echo "Execute emcc"
emcc -lembind -s LLD_REPORT_UNDEFINED --no-entry -s NO_DISABLE_EXCEPTION_CATCHING -o droplet.js -Wl,--whole-archive libdroplet.a -Wl,--no-whole-archive

echo "Copy compiled files to webassembly folder"
cp build/droplet.js webassembly/droplet.js
cp build/droplet.wasm webassembly/droplet.wasm
