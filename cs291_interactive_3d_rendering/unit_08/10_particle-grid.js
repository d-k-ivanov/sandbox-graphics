"use strict";

// Particle System

/*global THREE, document, window, $*/

var path = "";

var camera, scene, renderer;
var cameraControls;

var clock = new THREE.Clock();

function fillScene()
{
    scene = new THREE.Scene();
    var geometry = new THREE.BufferGeometry();

    // Student: rewrite the following vertex generation code so that
    // vertices are generated every 100 units:
    // -1000,-1000,-1000 to 1000,1000,1000, e.g.
    // at -1000,-1000,-1000, -900,-1000,-1000,
    // and so on, for the 21*21*21 = 9261 points.

    const vertices = []
    // for (var i = 0; i < 8000; i++)
    // {
    //     let vertex = new THREE.Vector3();
    //     // accept the point only if it's in the sphere
    //     do
    //     {
    //         vertex.x = 2000 * Math.random() - 1000;
    //         vertex.y = 2000 * Math.random() - 1000;
    //         vertex.z = 2000 * Math.random() - 1000;
    //     } while (vertex.length() > 1000);

    //     vertices.push(vertex);
    // }

    for (var x = -1000; x <= 1000; x += 100)
    {
        for (var y = -1000; y <= 1000; y += 100)
        {
            for (var z = -1000; z <= 1000; z += 100)
            {
                let vertex = new THREE.Vector3(x, y, z);
                vertices.push(vertex);
            }
        }
    }

    geometry.setFromPoints(vertices)

    var disk = THREE.ImageUtils.loadTexture(path + '../textures/disc.png');
    var material = new THREE.ParticleBasicMaterial(
        { size: 35, sizeAttenuation: false, map: disk, transparent: true });
    material.color.setHSL(0.9, 0.2, 0.6);

    var particles = new THREE.ParticleSystem(geometry, material);
    particles.sortParticles = true;
    scene.add(particles);
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer = new THREE.WebGLRenderer({ clearAlpha: 1 });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    // CAMERA
    camera = new THREE.PerspectiveCamera(55, canvasRatio, 2, 18000);
    camera.position.set(10, 5, 15);

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
    render();
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
