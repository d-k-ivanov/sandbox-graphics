"use strict";

// Procedural texturing, with gain
// Your task is to change the fragment shader, and only that
// Edit the file named fragment.glsl in the tab above.

/*global THREE, document, window, dat, $*/

var camera, scene, renderer, light, vs, fs;
var cameraControls, effectController, phongMaterial;
var clock = new THREE.Clock();

async function loadShadersAndRun()
{
    // Load Shaders
    vs = await (await fetch('unit_09/06_vertex.glsl')).text();
    fs = await (await fetch('unit_09/06_fragment.glsl')).text();

    init();
    fillScene();
    setupGui();
    addToDOM();
    animate();
}

function fillScene()
{
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0xAAAAAA, 2000, 4000);

    // LIGHTS
    var ambientLight = new THREE.AmbientLight(0x333333); // 0.2

    light = new THREE.DirectionalLight(0xFFFFFF, 1.0);
    light.position.set(320, 390, 700);

    scene.add(ambientLight);
    scene.add(light);

    // Calls that add things to scene (that should be graded) go here
    // Do not add here anything that is unrelated to exercise
    // Like grids or axes
    var teapotSize = 400;
    var materialColor = new THREE.Color();
    materialColor.setRGB(1.0, 0.8, 0.6);

    phongMaterial = createShaderMaterial("phongDiffuse", light);
    phongMaterial.uniforms.uMaterialColor.value.copy(materialColor);
    phongMaterial.side = THREE.DoubleSide;

    var teapot = new THREE.Mesh(new THREE.TeapotGeometry(teapotSize, 20, true, true, true, true), phongMaterial);

    scene.add(teapot);
}


function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 1, 80000);
    camera.position.set(-600, 900, 1300);

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

    // take inputs from sliders and modify shader's values
    phongMaterial.uniforms.uKd.value = effectController.kd;
    phongMaterial.uniforms.uScale.value = effectController.scale;
    phongMaterial.uniforms.uGainAlpha.value = effectController.gainAlpha;

    var materialColor = new THREE.Color();
    materialColor.setHSL(effectController.hue, effectController.saturation, effectController.lightness);
    phongMaterial.uniforms.uMaterialColor.value.copy(materialColor);

    light.position.set(effectController.lx, effectController.ly, effectController.lz);
    light.color.setHSL(effectController.lhue, effectController.lsaturation, effectController.llightness);

    renderer.render(scene, camera);
}

function createShaderMaterial(id, light)
{

    // could be a global, defined once, but here for convenience
    var shaderTypes = {
        'phongDiffuse': {
            uniforms: {
                "uDirLightPos": { type: "v3", value: new THREE.Vector3() },
                "uDirLightColor": { type: "c", value: new THREE.Color(0xFFFFFF) },
                "uMaterialColor": { type: "c", value: new THREE.Color(0xFFFFFF) },
                uKd: {
                    type: "f",
                    value: 0.7
                },
                uScale: {
                    type: "f",
                    value: 0.2
                },
                uGainAlpha: {
                    type: "f",
                    value: 0.1
                }
            }
        }
    };

    var shader = shaderTypes[id];
    var u = THREE.UniformsUtils.clone(shader.uniforms);

    var material = new THREE.ShaderMaterial({ uniforms: u, vertexShader: vs, fragmentShader: fs });
    material.uniforms.uDirLightPos.value = light.position;
    material.uniforms.uDirLightColor.value = light.color;

    return material;
}

function setupGui()
{

    effectController = {
        kd: 0.7,
        scale: 0.2,
        gainAlpha: 0.1,

        hue: 0.463,
        saturation: 0.46,
        lightness: 0.7,

        lhue: 0.04,
        lsaturation: 0.01,
        llightness: 0.7,

        // bizarrely, if you initialize these with negative numbers, the sliders
        // will not show any decimal places.
        lx: 0.32,
        ly: 0.39,
        lz: 0.7
    };

    var h;
    var gui = new dat.GUI();

    // material (attributes)
    h = gui.addFolder("Material control");
    h.add(effectController, "kd", 0.0, 1.0, 0.025).name("Kd");
    h.add(effectController, "scale", 0.01, 1.0, 0.025).name("function scale");
    h.add(effectController, "gainAlpha", 0.01, 0.99, 0.025).name("gain alpha");

    // material (color)
    h = gui.addFolder("Material color");
    h.add(effectController, "hue", 0.0, 1.0, 0.025).name("hue");
    h.add(effectController, "saturation", 0.0, 1.0, 0.025).name("saturation");
    h.add(effectController, "lightness", 0.0, 1.0, 0.025).name("value");

    // light (point)
    h = gui.addFolder("Light color");
    h.add(effectController, "lhue", 0.0, 1.0, 0.025).name("hue");
    h.add(effectController, "lsaturation", 0.0, 1.0, 0.025).name("saturation");
    h.add(effectController, "llightness", 0.0, 1.0, 0.025).name("lightness");

    // light (directional)
    h = gui.addFolder("Light direction");
    h.add(effectController, "lx", -1.0, 1.0, 0.025).name("x");
    h.add(effectController, "ly", -1.0, 1.0, 0.025).name("y");
    h.add(effectController, "lz", -1.0, 1.0, 0.025).name("z");
}

try
{
    loadShadersAndRun();
} catch (e)
{
    alert(e);
}
