"use strict";

// Texture animation

/*global THREE, Coordinates, document, window, dat, $*/

var path = ""; // STUDENT: set to "" to run on your computer, "/" for submitting code to Udacity

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();

var uX = 1;
var uY = 1;
var uU = 1;
var uV = 1;

var texture = [];

var material = [];
var mtlName = 'grid';
var wireMaterial;

var wrapName = 'repeat';
var wrapVal = THREE.RepeatWrapping;

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

function setWrap()
{
    for (var name in texture)
    {
        if (texture.hasOwnProperty(name))
        {
            texture[name].wrapS = wrapVal; texture[name].wrapT = wrapVal;
            // if you change wrap mode, you need to signal that texture needs update
            texture[name].needsUpdate = true;
        }
    }
}

function SquareGeometry()
{
    var geometry = new THREE.BufferGeometry();

    // generate vertices
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
    renderer.shadowMapEnabled = true;

    // Camera: Y up, X right, Z up
    camera = new THREE.PerspectiveCamera(1, canvasRatio, 10, 200);
    camera.position.set(0.75, 0.5, 100);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0.75, 0.5, 0);

    wireMaterial = new THREE.MeshBasicMaterial({ wireframe: true, color: 0x00FF00 });

    // TEXTURES
    const textureLoader = new THREE.TextureLoader();
    texture.crate = textureLoader.load(path + '../textures/crate.gif');
    texture.grid = textureLoader.load(path + '../textures/ash_uvgrid01.jpg');
    texture.maple = textureLoader.load(path + '../textures/maple.png');
    texture.feather = textureLoader.load(path + '../textures/feather.png');
    texture.water = textureLoader.load(path + '../textures/water.jpg');
    texture.concrete = textureLoader.load(path + '../textures/concrete.jpg');
    texture.letterR = textureLoader.load(path + '../textures/r_border.png');

    // MATERIALS
    for (var name in texture)
    {
        if (texture.hasOwnProperty(name))
        {
            material[name] = new THREE.MeshBasicMaterial({ map: texture[name], side: THREE.DoubleSide });
        }
    }

    setWrap();
    fillScene();
}

function animate()
{
    window.requestAnimationFrame(animate);
    var time = clock.getElapsedTime();
    texture[effectController.mtlName].offset.set(0, time);
    texture[effectController.mtlName].offset.set(0.2 * Math.sin(2 * time), time);
    texture[effectController.mtlName].repeat.set(effectController.repeat, effectController.repeat / 3);
    render();
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);

    if (effectController.reset)
    {
        resetGui();
        // Iterate over all controllers
        for (var i in gui.__controllers)
        {
            if (gui.__controllers.hasOwnProperty(i))
            {
                gui.__controllers[i].updateDisplay();
            }
        }
    }

    var refill = false;
    if (wrapName !== effectController.wrap)
    {
        wrapName = effectController.wrap;
        refill = true;

        if (effectController.wrap === 'repeat')
        {
            wrapVal = THREE.RepeatWrapping;
        } else if (effectController.wrap === 'mirrored repeat')
        {
            wrapVal = THREE.MirroredRepeatWrapping;
        } else
        {
            wrapVal = THREE.ClampToEdgeWrapping;
        }

        setWrap();
    }

    if (refill ||
        showPoly !== effectController.showPoly ||
        mtlName !== effectController.mtlName)
    {
        showPoly = effectController.showPoly;
        mtlName = effectController.mtlName;
        fillScene();
    }

    texture[effectController.mtlName].repeat.set(
        effectController.repeat, effectController.repeat);

    // Student:
    // Transform the texture here to move downwards at
    // a rate of one copy of the texture per second.

    renderer.render(scene, camera);
}

function resetGui()
{
    effectController = {
        wrap: 'repeat',
        repeat: 3,
        showPoly: false,
        mtlName: 'water',
        reset: false
    };
}

function addToDOM()
{
    var container = document.getElementById('container');
    var canvas = container.getElementsByTagName('canvas');
    if (canvas.length > 0)
    {
        container.removeChild(canvas[0]);
    }
    container.appendChild(renderer.domElement);
}

function setupGui()
{
    resetGui();
    gui = new dat.GUI();
    gui.add(effectController, "wrap", ['repeat', 'mirrored repeat', 'clamp to edge']).name("wrap mode");
    gui.add(effectController, "repeat", 0.0, 10.0).name("texture repeat");
    gui.add(effectController, "showPoly").name("show polygon");
    gui.add(effectController, "mtlName", ['crate', 'grid', 'water', 'concrete', 'letterR']).name("texture image");
    gui.add(effectController, "reset").name("reset");
}

try
{
    setupGui();
    init();
    fillScene();
    addToDOM();
    animate();
} catch (e)
{
    alert(e);
}
