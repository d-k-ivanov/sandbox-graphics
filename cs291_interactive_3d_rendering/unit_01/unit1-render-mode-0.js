"use strict";

// Rendering modes demo
// This is rendering mode #0
// Rotate the scene and see how the objects are displayed
/*global THREE, requestAnimationFrame, window, document, Stats, dat */

var container, camera, scene, renderer, stats;
var cameraControls, effectController;
var clock = new THREE.Clock();
var sphere, cube, cylinder;

function addToDOM()
{
    container = document.getElementById('container');
    var canvas = container.getElementsByTagName('canvas');
    if (canvas.length > 0)
    {
        container.removeChild(canvas[0]);
    }
    container.appendChild(renderer.domElement);
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight-50;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 4000);
    camera.position.set(200, 550, 1300);

    // SCENE
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0xFFFFFF, 1000, 4000);

    // LIGHTS
    scene.add(new THREE.AmbientLight(0x222222));
    var light = new THREE.DirectionalLight(0xFFFFFF, 2.25);
    light.position.set(200, 400, 500);
    scene.add(light);

    var light2 = new THREE.DirectionalLight(0xFFFFFF, 1.5);
    light2.position.set(-400, -200, 200);
    scene.add(light2);

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(scene.fog.color, 1);

    // Setting up rendering/sorting options
    renderer.sortObjects = true;
    renderer.getContext().depthFunc(renderer.getContext().LEQUAL);

    addToDOM();
    // STATS
    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '0px';
    stats.domElement.style.zIndex = 100;
    stats.domElement.style.color = "#aaa";
    stats.domElement.style.background = "transparent";
    container.appendChild(stats.domElement);

    cameraControls = new THREE.OrbitControls(camera, renderer.domElement);
    cameraControls.target.set(0, 50, 0);

    // MODELS
    sphere = new THREE.Mesh(
        new THREE.SphereGeometry(210, 32, 16),
        new THREE.MeshPhongMaterial({ color: 0x004000, specular: 0x606060 }));

    sphere.position.x = 0;
    sphere.position.y = 210;
    sphere.position.z = 0;

    scene.add(sphere);

    cube = new THREE.Mesh(
        new THREE.BoxGeometry(120, 380, 200),
        new THREE.MeshLambertMaterial({ color: 0x800000 }));

    cube.position.x = 50;
    cube.position.y = 190;
    cube.position.z = 400;

    scene.add(cube);

    cylinder = new THREE.Mesh(
        new THREE.CylinderGeometry(200, 200, 500, 32, 1),
        new THREE.MeshPhongMaterial({ color: 0x000060, specular: 0x000060 }));

    cylinder.position.x = 40;
    cylinder.position.y = 250;
    cylinder.position.z = -500;

    scene.add(cylinder);
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);
    renderer.render(scene, camera);
}

function animate()
{
    requestAnimationFrame(animate);
    render();
    stats.update();
}

init();
animate();
