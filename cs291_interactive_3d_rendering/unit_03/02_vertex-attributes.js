"use strict";

/*global THREE, window, document, $*/

var camera, scene, renderer;
var material, geometry, mesh;
var cameraControls;
var clock = new THREE.Clock();

function fillScene()
{
    scene = new THREE.Scene();
    // Triangle Mesh
    material = new THREE.MeshBasicMaterial({ vertexColors: THREE.VertexColors, side: THREE.DoubleSide });
    geometry = new THREE.BufferGeometry();

    // const vertices = new Float32Array([
    //     100, 0, 0,
    //     0, 100, 0,
    //     0, 0, 100
    // ]);
    // geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    const vertices = [];
    vertices.push(new THREE.Vector3(100, 0, 0));
    vertices.push(new THREE.Vector3(0, 100, 0));
    vertices.push(new THREE.Vector3(0, 0, 100));
    geometry.setFromPoints(vertices);

    const colors = [];
    let color_r = new THREE.Color(0xFF0000);
    let color_g = new THREE.Color(0x00FF00);
    let color_b = new THREE.Color(0x0000FF);
    colors.push(color_r.r, color_r.g, color_r.b);
    colors.push(color_g.r, color_g.g, color_g.b);
    colors.push(color_b.r, color_b.g, color_b.b);
    geometry.setAttribute('color', new THREE.Float32BufferAttribute(colors, 3));

    mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    // renderer.gammaInput = true;
    // renderer.outputEncoding = true;
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);

    // CAMERA
    camera = new THREE.PerspectiveCamera(55, canvasRatio, 1, 4000);
    camera.position.set(100, 150, 130);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);
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

function animate()
{
    window.requestAnimationFrame(animate);
    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.02;
    render(camera, scene);
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);
    renderer.render(scene, camera);
}

try
{
    init();
    fillScene();
    addToDOM();
    animate();
} catch (e)
{
    alert(e);
}
