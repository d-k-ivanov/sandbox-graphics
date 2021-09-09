mkdir build
mkdir js
mkdir models
mkdir textures
wget https://threejs.org/build/three.js -O js/three.js
wget https://threejs.org/build/three.module.js -O js/three.module.js
wget https://threejs.org/build/three.module.js -O build/three.module.js

wget https://raw.githubusercontent.com/mrdoob/three.js/dev/examples/jsm/controls/OrbitControls.js -O js/OrbitControls.js
wget https://raw.githubusercontent.com/mrdoob/three.js/dev/examples/jsm/loaders/GLTFLoader.js -O js/GLTFLoader.js
wget https://raw.githubusercontent.com/mrdoob/three.js/dev/examples/jsm/loaders/RGBELoader.js -O js/RGBELoader.js
wget https://raw.githubusercontent.com/mrdoob/three.js/dev/examples/jsm/utils/RoughnessMipmapper.js -O js/RoughnessMipmapper.js
