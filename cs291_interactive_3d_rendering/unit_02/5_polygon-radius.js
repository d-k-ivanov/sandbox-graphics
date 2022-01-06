"use strict";

// Polygon Radius Exercise
// Your task is to write a function that will take 3 arguments:
//   sides - how many edges the polygon has.
//   location - location of the center of the polygon as a THREE.Vector3.
//   radius - radius of the polygon.
// Return the mesh that defines the minimum number of triangles necessary
// to draw the polygon.

/*global THREE, Coordinates, $, document*/

var camera, scene, renderer;
var windowScale;

function PolygonGeometry(sides, location, radius)
{
    var geometry = new THREE.BufferGeometry();

    // Generate Vertices and Indices
    const vertices = []
    const indices = []
    for (let pt = 0; pt < sides; pt++)
    {
        // Add 90 degrees so we start at +Y axis, rotate counterclockwise around
        let angle = (Math.PI / 2) + (pt / sides) * 2 * Math.PI;
        let x = Math.cos(angle) * radius;
        let y = Math.sin(angle) * radius;

        // let vertex = new THREE.Vector3(x + location.x, y +location.y, 0 + location.z);
        let vertex = new THREE.Vector3(x, y, 0).add(location);
        vertices.push(vertex);
        if (pt >= 2)
        {
            indices.push(0, pt - 1, pt)
        }
    }
    geometry.setFromPoints(vertices)
    geometry.setIndex(indices);

    return geometry;
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;
    // scene
    scene = new THREE.Scene();

    windowScale = 12;
    var windowWidth = windowScale * canvasRatio;
    var windowHeight = windowScale;

    // Camera: Y up, X right, Z up
    camera = new THREE.OrthographicCamera(
        windowWidth / -2, windowWidth / 2,
        windowHeight / 2, windowHeight / -2,
        0, 40
    );

    var focus = new THREE.Vector3(5, 5, 0);
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
    // Background grid and axes. Grid step size is 1, axes cross at 0, 0
    Coordinates.drawGrid({ size: 100, scale: 1, orientation: "z" });
    Coordinates.drawAxes({ axisLength: 15, axisOrientation: "x", axisRadius: 0.02 });
    Coordinates.drawAxes({ axisLength: 11, axisOrientation: "y", axisRadius: 0.02 });
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
    // var geo = PolygonGeometry(9, new THREE.Vector3(5, 5, 0), 4);
    var geo = PolygonGeometry(9, new THREE.Vector3(5, 5, 0), 3);
    var material = new THREE.MeshBasicMaterial({ color: 0x0066ff, side: THREE.FrontSide });
    var mesh = new THREE.Mesh(geo, material);
    scene.add(mesh);
    addToDOM();
    render();
} catch (e)
{
    alert(e);
}
