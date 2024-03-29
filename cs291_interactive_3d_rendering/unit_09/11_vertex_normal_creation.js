"use strict";

// Vertex normal creation:
// You have to modify only the vertex shader in this exercise.
// It can be found in the file vertex.glsl in the tabs above.

/*global THREE, requestAnimationFrame, dat, $ */

var camera, scene, renderer, vs, fs;
var cameraControls;
var effectController;
var clock = new THREE.Clock();
var tess = 7, newTess = tess;
var ambientLight, light;
var phongBalancedMaterial;

async function loadShadersAndRun()
{
    // Load Shaders
    vs = await (await fetch('unit_09/11_vertex.glsl')).text();
    fs = await (await fetch('unit_09/11_fragment.glsl')).text();

    init();
    fillScene();
    setupGui();
    addToDOM();
    animate();
}

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // LIGHTS
    ambientLight = new THREE.AmbientLight(0x333333); // 0.2
    light = new THREE.DirectionalLight(0xffffff, 1.0);
    // direction is set by controller

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 0.001, 80);
    camera.position.set(-1.41, 0.14, -0.56);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, -0.45, 0);

    // MATERIALS
    // Note: setting per pixel off does not affect the specular highlight;
    // it affects only whether the light direction is recalculated each pixel.
    var materialColor = new THREE.Color();
    materialColor.setRGB(1.0, 0.8, 0.6);

    phongBalancedMaterial = createShaderMaterial("phongBalanced", light, ambientLight);
    phongBalancedMaterial.uniforms.uMaterialColor.value.copy(materialColor);
    phongBalancedMaterial.side = THREE.DoubleSide;

}

function createShaderMaterial(id, light, ambientLight)
{
    var shaderTypes = {
        'phongBalanced': {
            uniforms: {
                "uDirLightPos": { type: "v3", value: new THREE.Vector3() },
                "uDirLightColor": { type: "c", value: new THREE.Color(0xffffff) },
                "uAmbientLightColor": { type: "c", value: new THREE.Color(0x050505) },
                "uMaterialColor": { type: "c", value: new THREE.Color(0xffffff) },
                "uSpecularColor": { type: "c", value: new THREE.Color(0xffffff) },
                uKd: {
                    type: "f",
                    value: 0.7
                },
                uKs: {
                    type: "f",
                    value: 0.3
                },
                shininess: {
                    type: "f",
                    value: 100.0
                },
                uSphereRadius2: {
                    type: "f",
                    value: 1.0
                }
            }
        }

    };

    var shader = shaderTypes[id];
    var u = THREE.UniformsUtils.clone(shader.uniforms);

    var material = new THREE.ShaderMaterial({ uniforms: u, vertexShader: vs, fragmentShader: fs });
    material.uniforms.uDirLightPos.value = light.position;
    material.uniforms.uDirLightColor.value = light.color;
    material.uniforms.uAmbientLightColor.value = ambientLight.color;

    return material;
}

function setupGui()
{
    effectController = {
        sphereRadius: 1.0,
        shininess: 20.0,
        ka: 0.2,
        kd: 0.4,
        ks: 0.35,
        metallic: false,

        hue: 0.09,
        saturation: 0.46,
        lightness: 0.7,

        lhue: 0.04,
        lsaturation: 0.0,
        llightness: 0.7,

        // bizarrely, if you initialize these with negative numbers, the sliders
        // will not show any decimal places.
        lx: 0.01,
        ly: 1,
        lz: 0.01
    };

    var h;
    var gui = new dat.GUI();

    // material (attributes)
    h = gui.addFolder("Material control");

    h.add(effectController, "sphereRadius", 0.7072, 10.0, 1.0).name("sphere radius");
    h.add(effectController, "shininess", 1.0, 100.0, 1.0).name("shininess");
    h.add(effectController, "ka", 0.0, 1.0, 0.025).name("m_ka");
    h.add(effectController, "kd", 0.0, 1.0, 0.025).name("m_kd");
    h.add(effectController, "ks", 0.0, 1.0, 0.025).name("m_ks");
    h.add(effectController, "metallic");
    // TODO: add tess for tessellation

    // material (color)
    h = gui.addFolder("Material color");

    h.add(effectController, "hue", 0.0, 1.0, 0.025).name("m_hue");
    h.add(effectController, "saturation", 0.0, 1.0, 0.025).name("m_saturation");
    h.add(effectController, "lightness", 0.0, 1.0, 0.025).name("m_lightness");

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

function animate()
{
    requestAnimationFrame(animate);
    render();
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);

    if (newTess !== tess)
    {
        tess = newTess;
        fillScene();
    }

    phongBalancedMaterial.uniforms.uSphereRadius2.value = effectController.sphereRadius * effectController.sphereRadius;
    phongBalancedMaterial.uniforms.shininess.value = effectController.shininess;
    phongBalancedMaterial.uniforms.uKd.value = effectController.kd;
    phongBalancedMaterial.uniforms.uKs.value = effectController.ks;

    var materialColor = new THREE.Color();
    materialColor.setHSL(effectController.hue, effectController.saturation, effectController.lightness);
    phongBalancedMaterial.uniforms.uMaterialColor.value.copy(materialColor);

    if (!effectController.metallic)
    {
        materialColor.setRGB(1, 1, 1);
    }
    phongBalancedMaterial.uniforms.uSpecularColor.value.copy(materialColor);

    // Ambient is just material's color times ka, light color is not involved
    ambientLight.color.setHSL(effectController.hue, effectController.saturation, effectController.lightness * effectController.ka);
    light.position.set(effectController.lx, effectController.ly, effectController.lz);
    light.color.setHSL(effectController.lhue, effectController.lsaturation, effectController.llightness);

    renderer.render(scene, camera);
}

function fillScene()
{
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0x808080, 2000, 4000);

    scene.add(camera);

    scene.add(ambientLight);
    scene.add(light);

    var solidGround = new THREE.Mesh(new THREE.PlaneGeometry(1, 1, 100, 100), phongBalancedMaterial);
    solidGround.rotation.x = - Math.PI / 2;
    scene.add(solidGround);
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

try
{
    loadShadersAndRun();
} catch (e)
{
    alert(e);
}
