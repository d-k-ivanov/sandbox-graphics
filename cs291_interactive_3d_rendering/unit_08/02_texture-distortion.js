"use strict";

// Adjust textured upper corners demo


/*global THREE, Coordinates, document, window, dat*/

var path = "";

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();

var ulX = 0;
var ulY = 1;
var ulU = 0;
var ulV = 1;

var urX = 1;
var urY = 1;
var urU = 1;
var urV = 1;

var crateTexture;
var gridTexture;
var mapleTexture;
var featherTexture;

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

    const vertices = new Float32Array([
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        urX, urY, 0.0,
        ulX, ulY, 0.0,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    const indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));

    const uvs = new Float32Array([
        0.0, 0.0,
        1.0, 0.0,
        urU, urV,
        ulU, ulV,
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

    // MATERIALS
    material.crate = new THREE.MeshBasicMaterial({ map: crateTexture });
    material.grid = new THREE.MeshBasicMaterial({ map: gridTexture });
    material.maple = new THREE.MeshBasicMaterial({ map: mapleTexture, transparent: true });
    material.feather = new THREE.MeshBasicMaterial({ map: featherTexture, transparent: true });

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
        effectController.ulX = 0.1;
        effectController.ulY = 1;
        effectController.ulU = 0.1;
        effectController.ulV = 1;

        effectController.urX = 1;
        effectController.urY = 1;
        effectController.urU = 1;
        effectController.urV = 1;

        effectController.reset = false;

        // Iterate over all controllers
        for (var i in gui.__controllers)
        {
            gui.__controllers[i].updateDisplay();
        }
    }
    if (ulX !== effectController.ulX ||
        ulY !== effectController.ulY ||
        ulU !== effectController.ulU ||
        ulV !== effectController.ulV ||
        urX !== effectController.urX ||
        urY !== effectController.urY ||
        urU !== effectController.urU ||
        urV !== effectController.urV ||
        showPoly !== effectController.showPoly ||
        mtlName !== effectController.mtlName)
    {
        ulX = effectController.ulX;
        ulY = effectController.ulY;
        ulU = effectController.ulU;
        ulV = effectController.ulV;
        urX = effectController.urX;
        urY = effectController.urY;
        urU = effectController.urU;
        urV = effectController.urV;
        showPoly = effectController.showPoly;
        mtlName = effectController.mtlName;
        fillScene();
    }

    renderer.render(scene, camera);
}

function setupGui()
{

    effectController = {
        ulX: 0.1,
        ulY: 1,
        ulU: 0.1,
        ulV: 1,

        urX: 1,
        urY: 1,
        urU: 1,
        urV: 1,

        showPoly: false,
        mtlName: 'grid',
        reset: false
    };

    gui = new dat.GUI();
    gui.add(effectController, "ulX", -0.5, 0.5).name("upper left X");
    gui.add(effectController, "ulY", 0.5, 1.5).name("upper left Y");
    gui.add(effectController, "ulU", -4.0, 0.5).name("upper left U");
    gui.add(effectController, "ulV", 0.5, 5.0).name("upper left V");
    gui.add(effectController, "urX", 0.5, 1.5).name("upper right X");
    gui.add(effectController, "urY", 0.5, 1.5).name("upper right Y");
    gui.add(effectController, "urU", 0.5, 5.0).name("upper right U");
    gui.add(effectController, "urV", 0.5, 5.0).name("upper right V");
    gui.add(effectController, "showPoly").name("show polygon");
    gui.add(effectController, "mtlName", ['crate', 'grid', 'maple', 'feather']).name("texture image");
    gui.add(effectController, "reset").name("reset");
}

try
{
    setupGui();
    init();
    effectController.reset = true;
    animate();
} catch (e)
{
    alert(e);
}
