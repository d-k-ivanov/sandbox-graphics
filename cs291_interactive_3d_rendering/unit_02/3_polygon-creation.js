"use strict";

// Polygon Creation Exercise
// Your task is to complete the function PolygonGeometry(sides)
// which takes 1 argument: sides
// Return the mesh that defines the minimum number of triangles necessary to draw the polygon.
// Radius of the polygon is 1. Center of the polygon is at 0, 0.

/*global THREE, Coordinates, $, document*/

var camera, scene, renderer;
var windowScale;

function PolygonGeometry(sides)
{
    var geometry = new THREE.BufferGeometry();

    // generate vertices
    const vertices = []
    // const indices = []
    for (var pt = 0; pt < sides; pt++)
    {
        // Add 90 degrees so we start at +Y axis, rotate counterclockwise around
        var angle = (Math.PI / 2) + (pt / sides) * 2 * Math.PI;
        var x = Math.cos(angle);
        var y = Math.sin(angle);
        // console.log("X:" + x + ",\t "+ "Y:" + y);
        vertices.push(new THREE.Vector3(x, y, 0));
    }
    // console.log(vertices);
    geometry.setFromPoints(vertices)

    const indices = [
        0, 1, 2,
        0, 2, 3,
        0, 3, 4
    ]
    geometry.setIndex(indices);

    return geometry;
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight-50;
    var canvasRatio = canvasWidth / canvasHeight;
    // scene
    scene = new THREE.Scene();

    // Camera: Y up, X right, Z up
    windowScale = 4;
    var windowWidth = windowScale * canvasRatio;
    var windowHeight = windowScale;

    camera = new THREE.OrthographicCamera(
        windowWidth / -2, windowWidth / 2,
        windowHeight / 2, windowHeight / -2,
        0, 40
    );

    var focus = new THREE.Vector3(0, 1, 0);
    camera.position.x = focus.x;
    camera.position.y = focus.y;
    camera.position.z = 10;
    camera.lookAt(focus);

    renderer = new THREE.WebGLRenderer({ antialias: false, preserveDrawingBuffer: true });
    renderer.gammaInput = true;
    renderer.gammaOutput = true;
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xFFFFFF), 1.0);
}

function showGrids()
{
    Coordinates.drawGrid({ size: 100, scale: 1, orientation: "z" });
    Coordinates.drawAxes({ axisLength: 4, axisOrientation: "x", axisRadius: 0.02 });
    Coordinates.drawAxes({ axisLength: 3, axisOrientation: "y", axisRadius: 0.02 });
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

function render()
{
    renderer.render(scene, camera);
}

try
{
    init();
    showGrids();
    var geo = PolygonGeometry(5);
    var material = new THREE.MeshBasicMaterial({ color: 0xff0000, side: THREE.FrontSide });
    var mesh = new THREE.Mesh(geo, material);
    scene.add(mesh);
    addToDOM();
    render();
} catch (e)
{
    alert(e);
}
