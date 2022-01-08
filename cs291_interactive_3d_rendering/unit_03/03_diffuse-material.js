"use strict";

// Diffuse material exercise

/*global THREE, window, document, $*/

var camera, scene, renderer;
var cameraControls;
var clock = new THREE.Clock();
var ambientLight, light;

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 1, 80000);
    camera.position.set(-300, 300, -1000);
    camera.lookAt(0, 0, 0);

    // LIGHTS
    // ambientLight = new THREE.AmbientLight(0xFFFFFF);
    ambientLight = new THREE.AmbientLight(0x0F0F0F);
    light = new THREE.DirectionalLight(0xFFFFFF, 0.7);
    light.position.set(-800, 900, 300);

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);

    renderer.gammaInput = true;
    renderer.gammaOutput = true;

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);
}

function createBall()
{
    // Do not change the color itself, change the material and use the ambient and diffuse components.
    var material = new THREE.MeshLambertMaterial({ color: 0x80FC66 * 0.4, shading: THREE.FlatShading });
    // let m_color_r = material.color.r;
    // let m_color_g = material.color.g;
    // let m_color_b = material.color.b;
    material.emissive = new THREE.Color(0x000000);
    var sphere = new THREE.Mesh(new THREE.SphereGeometry(400, 64, 32), material);
    return sphere;
}

function fillScene()
{
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0x808080, 2000, 4000);

    // LIGHTS
    scene.add(ambientLight);
    scene.add(light);

    var ball = createBall();
    scene.add(ball);

    // Coordinates.drawGround({size:1000});
    // Coordinates.drawGrid({size:1000,scale:0.01});
    // Coordinates.drawAllAxes({ axisLength: 500, axisRadius: 1, axisTess: 4 });
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
