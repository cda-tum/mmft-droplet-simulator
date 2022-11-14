# MMFT Droplet Simulator
This branch is meant to be used for compiling the MMFT Droplet Simulator to WebAssembly. For the readme of the actual Simulator please refer to the master branch.

## Compile to WebAssembly
Please note, that the following steps were only tested on Linux.
1. Make sure that the Droplet Simulator can be build without errors, as described in the readme of the master branch.
2. [Install Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and make sure that it is added to the PATH variable.
3. Run the following
```bash
./myCmake.sh
./myMake.sh
./myEmcc.sh
```
1. If no errors occurred a "droplet.js" and a "droplet.wasm" file should now be inside the "webassembly" folder.

## Use compiled file
In order to use the compiled "droplet.wasm" file, the "droplet.js" file has to be imported into a javascript/typescript file, which contains all the "glue" code to load the wasm file. How this can be accomplished is shown in the following as well as in the javascript and typescript folders. 

### Javascript
1. Copy the compiled "droplet.js" and "droplet.wasm" files into the javascript folder.
2. Import the "droplet.js" into your javascript file, create a simulator and use it like the C++ API (example file provided in folder):
```javascript
var dropletLib = require('./droplet.js')

dropletLib.onRuntimeInitialized = function () {
  //create new simulator
  const sim = new dropletLib.Simulator();

  //use simulator ...
}

```
3. Execute the javascript file.

### Typescript
In order to use typescript please make sure that [Node.js](https://nodejs.org/en/) and [npm](https://www.npmjs.com/) (usually comes with Node.js) is correctly installed. Furthermore, execute the following steps inside the typescript folder.
1. Run `npm install`.
2. Copy the compiled "droplet.js" and "droplet.wasm" files into the "dist" folder (create the folder if it does not exist).
3. Write a "main.ts" typescript file (example file provided in "src" folder) like:
```typescript
import dropletLib from './droplet';
import { SimulationResult } from './Interfaces';

dropletLib.onRuntimeInitialized = function () {
  //create new simulator
  const sim = new dropletLib.Simulator();

  //use simulator ...
}

```
4. Compile the typescript files with `npx tsc`.
5. Run the compiled files with `node dist/main.js`.
