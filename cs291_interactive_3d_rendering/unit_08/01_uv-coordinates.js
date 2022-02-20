"use strict";

// Adjust textured rectangle demo

/*global THREE, Coordinates, document, window, dat*/

var path = "";

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();

var uX = 1;
var uY = 1;
var uU = 1;
var uV = 1;

var crateTexture;
var gridTexture;
var mapleTexture;
var featherTexture;
var darkamTexture;

var material = [];
var mtlName = 'grid';
var wireMaterial;

var gui;

var showPoly = false;

function fillScene()
{
    scene = new THREE.Scene();

    // Background grid and axes. Grid step size is 1, axes cross at 0, 0
    Coordinates.drawGrid({ size: 100, scale: 1, orientation: "z", offset: -0.01 });
    Coordinates.drawAxes({ axisLength: 2.1, axisOrientation: "x", axisRadius: 0.004, offset: -0.01 });
    Coordinates.drawAxes({ axisLength: 2.1, axisOrientation: "y", axisRadius: 0.004, offset: -0.01 });

    var myPolygon = new SquareGeometry();
    var polygonObject = new THREE.Mesh(myPolygon, material[mtlName]);
    scene.add(polygonObject);

    if (effectController.showPoly)
    {
        polygonObject = new THREE.Mesh(myPolygon, wireMaterial);
        scene.add(polygonObject);
    }
}

function SquareGeometry()
{
    var geometry = new THREE.BufferGeometry();

    // generate vertices
    // const vertices = []
    // vertices.push(new THREE.Vector3(0.0, 0.0, 0.0));
    // vertices.push(new THREE.Vector3(uX, 0.0, 0.0));
    // vertices.push(new THREE.Vector3(uX, uY, 0.0));
    // vertices.push(new THREE.Vector3(0.0, uY, 0.0));
    // geometry.setFromPoints(vertices)
    const vertices = new Float32Array([
        0.0, 0.0, 0.0,
        uX, 0.0, 0.0,
        uX, uY, 0.0,
        0.0, uY, 0.0,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    // generate indexes
    const indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));

    // generate uv coordinates
    // const uvs = [];
    // uvs.push(new THREE.Vector2(0.0, 0.0));
    // uvs.push(new THREE.Vector2(uU, 0.0));
    // uvs.push(new THREE.Vector2(uU, uV));
    // uvs.push(new THREE.Vector2(0.0, uV));
    const uvs = new Float32Array([
        0.0, 0.0,
        uU, 0.0,
        uU, uV,
        0.0, uV,
    ]);
    geometry.setAttribute('uv', new THREE.BufferAttribute(uvs, 2));

    return geometry;
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xFFFFFF), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;


    var container = document.getElementById('container');
    container.appendChild(renderer.domElement);

    // Camera: Y up, X right, Z up
    camera = new THREE.PerspectiveCamera(1.1, canvasRatio, 10, 200);
    camera.position.set(0.8, 0.6, 100);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0.8, 0.6, 0);

    // TEXTURES
    const textureLoader = new THREE.TextureLoader();
    crateTexture = textureLoader.load(path + '../textures/crate.gif');
    crateTexture.wrapS = THREE.RepeatWrapping; crateTexture.wrapT = THREE.RepeatWrapping;
    gridTexture = textureLoader.load(path + '../textures/ash_uvgrid01.jpg');
    gridTexture.wrapS = THREE.RepeatWrapping; gridTexture.wrapT = THREE.RepeatWrapping;
    mapleTexture = textureLoader.load(path + '../textures/maple.png');
    mapleTexture.wrapS = THREE.RepeatWrapping; mapleTexture.wrapT = THREE.RepeatWrapping;
    featherTexture = textureLoader.load(path + '../textures/feather.png');
    featherTexture.wrapS = THREE.RepeatWrapping; featherTexture.wrapT = THREE.RepeatWrapping;
    darkamTexture = textureLoader.load(path + '../textures/darkam.png');
    darkamTexture.wrapS = THREE.RepeatWrapping; darkamTexture.wrapT = THREE.RepeatWrapping;

    // MATERIALS
    material.crate = new THREE.MeshBasicMaterial({ map: crateTexture });
    material.grid = new THREE.MeshBasicMaterial({ map: gridTexture });
    material.maple = new THREE.MeshBasicMaterial({ map: mapleTexture, transparent: true });
    material.feather = new THREE.MeshBasicMaterial({ map: featherTexture, transparent: true });
    material.darkam = new THREE.MeshBasicMaterial({ map: darkamTexture });

    wireMaterial = new THREE.MeshBasicMaterial({ wireframe: true, color: 0x00FF00 });

    fillScene();
}

function animate()
{
    window.requestAnimationFrame(animate);
    render();
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);

    if (effectController.reset)
    {
        // TODO: defect with dat.gui is that the 0's sliders don't show up correctly.
        effectController.uX = 1;
        effectController.uY = 1;
        effectController.uU = 1;
        effectController.uV = 1;

        effectController.reset = false;

        // Iterate over all controllers
        for (var i in gui.__controllers)
        {
            if (gui.__controllers.hasOwnProperty(i))
            {
                gui.__controllers[i].updateDisplay();
            }
        }
    }
    if (uX !== effectController.uX ||
        uY !== effectController.uY ||
        uU !== effectController.uU ||
        uV !== effectController.uV ||
        showPoly !== effectController.showPoly ||
        mtlName !== effectController.mtlName)
    {
        uX = effectController.uX;
        uY = effectController.uY;
        uU = effectController.uU;
        uV = effectController.uV;
        showPoly = effectController.showPoly;
        mtlName = effectController.mtlName;
        fillScene();
    }

    renderer.render(scene, camera);
}

function setupGui()
{

    effectController = {
        uX: 1,
        uY: 1,
        uU: 1,
        uV: 1,
        showPoly: false,
        mtlName: 'grid',
        reset: false
    };

    gui = new dat.GUI();
    gui.add(effectController, "uX", 0.0, 1.5).name("right X");
    gui.add(effectController, "uY", 0.0, 1.5).name("upper Y");
    gui.add(effectController, "uU", 0.0, 3.0).name("right U");
    gui.add(effectController, "uV", 0.0, 3.0).name("upper V");
    gui.add(effectController, "showPoly").name("show polygon");
    gui.add(effectController, "mtlName", ['crate', 'grid', 'maple', 'feather', 'darkam']).name("texture image");
    gui.add(effectController, "reset").name("reset");
}

try
{
    setupGui();
    init();
    animate();
} catch (e)
{
    alert(e);
}
