"use strict";

// Add a reflection map

/*global THREE, requestAnimationFrame, $ */

var camera, scene, renderer;
var cameraControls;
var clock = new THREE.Clock();
var teapotSize = 400;

function fillScene()
{
    scene = new THREE.Scene();

    // LIGHTS
    scene.add(new THREE.AmbientLight(0x333333));

    var light = new THREE.DirectionalLight(0xFFFFFF, 0.9);
    light.position.set(-1300, 700, 1240);

    scene.add(light);

    light = new THREE.DirectionalLight(0xFFFFFF, 0.7);
    light.position.set(1000, -500, -1200);

    scene.add(light);

    // SKY BOX
    var textureCube = new THREE.CubeTextureLoader()
        .setPath('../textures/skybox/')
        .load([
            'px.jpg',
            'nx.jpg',
            'py.jpg',
            'ny.jpg',
            'pz.jpg',
            'nz.jpg'
        ]);
    textureCube.format = THREE.RGBFormat;

    var uniforms = THREE.UniformsUtils.clone(THREE.ShaderLib.cube.uniforms);
    uniforms.envMap.value = textureCube;

    var skyBoxMaterial = new THREE.ShaderMaterial({
        fragmentShader: THREE.ShaderLib.cube.fragmentShader,
        vertexShader: THREE.ShaderLib.cube.vertexShader,
        uniforms: uniforms,
        depthWrite: false,
        side: THREE.BackSide
    });

    Object.defineProperty(skyBoxMaterial, 'envMap', {
        get: function ()
        {
            return textureCube;
        }
    });

    var sky = new THREE.Mesh(new THREE.BoxGeometry(5000, 5000, 5000), skyBoxMaterial);
    scene.add(sky);

    // TEAPOT
    var teapotMaterial = new THREE.MeshPhongMaterial({ color: 0x770000, specular: 0xffaaaa, envMap: textureCube });
    var teapot = new THREE.Mesh(new THREE.TeapotGeometry(teapotSize, 8, true, true, true, true, true), teapotMaterial);
    scene.add(teapot);
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 100, 20000);
    camera.position.set(-222, 494, 1746);

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, -160, 0);
}

// EVENT HANDLERS
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
    requestAnimationFrame(animate);
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
