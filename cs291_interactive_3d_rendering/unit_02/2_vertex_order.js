"use strict";

// Vertex Order Exercise
// Your task is to determine the problem and fix the vertex drawing order.
// Check the function someObject()
// and correct the code that starts at line 17.

/*global THREE, Coordinates, $, document*/

var camera, scene, renderer;
var windowScale;

function someObject(material)
{
    var geometry = new THREE.BufferGeometry();

    const vertices = []
    vertices.push(new THREE.Vector3(3, 3, 0));
    vertices.push(new THREE.Vector3(7, 3, 0));
    vertices.push(new THREE.Vector3(7, 7, 0));
    vertices.push(new THREE.Vector3(3, 7, 0));
    geometry.setFromPoints(vertices)

    const indices = [
        0, 1, 2,
        // 2, 0, 3 - backside triangle
        3, 0, 2
    ]
    geometry.setIndex(indices);

    var mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);
}

function init()
{
    // var canvasWidth = 846;
    // var canvasHeight = 494;
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight;
    var canvasRatio = canvasWidth / canvasHeight;
    // scene
    scene = new THREE.Scene();

    // Camera: Y up, X right, Z up
    windowScale = 10;
    var windowWidth = windowScale * canvasRatio;
    var windowHeight = windowScale;

    camera = new THREE.OrthographicCamera(windowWidth / - 2, windowWidth / 2,
        windowHeight / 2, windowHeight / - 2, 0, 40);

    var focus = new THREE.Vector3(5, 4, 0);
    camera.position.x = focus.x;
    camera.position.y = focus.y;
    camera.position.z = 10;
    camera.lookAt(focus);

    renderer = new THREE.WebGLRenderer({ antialias: true, preserveDrawingBuffer: true });
    renderer.gammaInput = true;
    renderer.gammaOutput = true;
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xFFFFFF), 1.0);
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

function showGrids()
{
    // Background grid and axes. Grid step size is 1, axes cross at 0, 0
    Coordinates.drawGrid({ size: 100, scale: 1, orientation: "z" });
    Coordinates.drawAxes({ axisLength: 11, axisOrientation: "x", axisRadius: 0.04 });
    Coordinates.drawAxes({ axisLength: 11, axisOrientation: "y", axisRadius: 0.04 });
}

function render()
{
    renderer.render(scene, camera);
}

// Main body of the script
try
{
    init();
    showGrids();
    var material = new THREE.MeshBasicMaterial({ color: 0x9BEF97, side: THREE.FrontSide });
    someObject(material);
    addToDOM();
    render();
} catch (e)
{
    alert(e);
}
