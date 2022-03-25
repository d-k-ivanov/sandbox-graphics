"use strict";

// Pick a letter, or actually - the number 1
// Edit the UV values in the SquareGeometry function
// to select the number "1" from the texture.
// Only the array 'uvs' should be modified.

/*global THREE, Coordinates, $, document, window*/

var path = "";

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();

function SquareGeometry()
{
    var geometry = new THREE.BufferGeometry();

    // generate vertices
    const vertices = new Float32Array([
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    // generate indexes
    const indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));

    // generate uv coordinates
    // const uvs = new Float32Array([
    //     0.0, 0.0,
    //     1.0, 0.0,
    //     1.0, 1.0,
    //     0.0, 1.0,
    // ]);
    const uvs = new Float32Array([
        0.75, 0.25,
        1.0, 0.25,
        1.0, 0.5,
        0.75, 0.5,
    ]);
    geometry.setAttribute('uv', new THREE.BufferAttribute(uvs, 2));

    return geometry;
}

function fillScene()
{
    scene = new THREE.Scene();
    var myPolygon = new SquareGeometry();
    const textureLoader = new THREE.TextureLoader();
    var myTexture = textureLoader.load(path + '../textures/lettergrid.png');
    var myPolygonMaterial = new THREE.MeshBasicMaterial({ map: myTexture });
    var polygonObject = new THREE.Mesh(myPolygon, myPolygonMaterial);
    scene.add(polygonObject);
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

    // Camera: Y up, X right, Z up
    camera = new THREE.PerspectiveCamera(1, canvasRatio, 50, 150);
    camera.position.set(0.5, 0.5, 100);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0.5, 0.5, 0);
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

function drawHelpers()
{
    // Background grid and axes. Grid step size is 1, axes cross at 0, 0
    Coordinates.drawGrid({ size: 100, scale: 1, orientation: "z", offset: -0.01 });
    Coordinates.drawAxes({ axisLength: 2.1, axisOrientation: "x", axisRadius: 0.004, offset: -0.01 });
    Coordinates.drawAxes({ axisLength: 2.1, axisOrientation: "y", axisRadius: 0.004, offset: -0.01 });
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

    renderer.render(scene, camera);
}

function setupGui()
{
    effectController = {
        alpha: 0.7,
        sred: 0xE5 / 255,
        sgreen: 0x33 / 255,
        sblue: 0x19 / 155,

        dred: 0xE5 / 255,
        dgreen: 0xE5 / 255,
        dblue: 0x66 / 255
    };
}

try
{
    init();
    fillScene();
    setupGui();
    drawHelpers();
    addToDOM();
    animate();
} catch (e)
{
    alert(e);
}
