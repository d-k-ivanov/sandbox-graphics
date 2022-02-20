"use strict"; // good practice - see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
////////////////////////////////////////////////////////////////////////////////
// Minification demo
////////////////////////////////////////////////////////////////////////////////
/*global THREE, document, window, dat*/

var path = "";	// STUDENT: set to "" to run on your computer, "/" for submitting code to Udacity

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();

var uX = 100;
var uY = 100;
var uU = 100;
var uV = 100;

var texture = [];

var material = [];
var mtlName = 'checker 2x2';

var wrapVal = THREE.RepeatWrapping;

var magName = 'nearest';
var magVal = THREE.NearestFilter;

var minName = 'nearest';
var minVal = THREE.NearestFilter;

var anisotropy = 1;

var gui;

function fillScene()
{
    scene = new THREE.Scene();

    var myPolygon = new SquareGeometry();
    var polygonObject = new THREE.Mesh(myPolygon, material[mtlName]);
    scene.add(polygonObject);

}

function setFilters()
{

    // MATERIALS
    for (var name in texture)
    {
        if (texture.hasOwnProperty(name))
        {
            texture[name].magFilter = magVal;
            texture[name].minFilter = minVal;
            texture[name].anisotropy = anisotropy;
            texture[name].wrapS = wrapVal; texture[name].wrapT = wrapVal;
            // if you change filtering, you need to signal that texture needs update
            texture[name].needsUpdate = true;
        }
    }
}

function SquareGeometry()
{
    var geometry = new THREE.BufferGeometry();
    const vertices = new Float32Array([
        -uX, 0.0, -uY,
        uX, 0.0, -uY,
        uX, 0.0, uY,
        -uX, 0.0, uY,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    const indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));

    const uvs = new Float32Array([
        uU, uV,
        uU, 0.0,
        0.0, 0.0,
        0.0, uV,
    ]);
    geometry.setAttribute('uv', new THREE.BufferAttribute(uvs, 2));

    return geometry;
}

// need to init this before GUI
function initRenderer()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    //renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer = new THREE.WebGLRenderer();
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xFFFFFF), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    var container = document.getElementById('container');
    container.appendChild(renderer.domElement);

    // Camera: Y up, X right, Z up
    camera = new THREE.PerspectiveCamera(40, canvasRatio, 0.1, 100);
    camera.position.set(10, 3.4, 2);

    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);
}

function init()
{
    // TEXTURES
    // If you change the magnify mode, you must reload texture?
    const textureLoader = new THREE.TextureLoader();
    texture['checker 1x1 (gray)'] = textureLoader.load(path + '../textures/checker1x1.png');
    texture['checker 2x2'] = textureLoader.load(path + '../textures/checker2x2.png');
    texture['checker 4x4'] = textureLoader.load(path + '../textures/checker4x4.png');
    texture['checker 8x8'] = textureLoader.load(path + '../textures/checker8x8.png');
    texture['checker 16x16'] = textureLoader.load(path + '../textures/checker16x16.png');
    texture['checker 32x32'] = textureLoader.load(path + '../textures/checker32x32.png');
    texture['checker 64x64'] = textureLoader.load(path + '../textures/checker64x64.png');
    texture['checker 128x128'] = textureLoader.load(path + '../textures/checker128x128.png');
    texture['checker 256x256'] = textureLoader.load(path + '../textures/checker256x256.png');
    texture['checker 512x512'] = textureLoader.load(path + '../textures/checker512x512.png');
    texture.crate = textureLoader.load(path + '../textures/crate.gif');
    texture.grid = textureLoader.load(path + '../textures/ash_uvgrid01.jpg');
    texture.water = textureLoader.load(path + '../textures/water.jpg');
    texture.concrete = textureLoader.load(path + '../textures/concrete.jpg');
    texture.letterR = textureLoader.load(path + '../textures/r_border.png');

    setFilters();

    for (var name in texture)
    {
        if (texture.hasOwnProperty(name))
        {
            material[name] = new THREE.MeshBasicMaterial({ map: texture[name], side: THREE.DoubleSide });
        }
    }

    fillScene();
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

    if (effectController.reset)
    {
        resetGui();

        // Iterate over all controllers
        for (var i in gui.__controllers)
        {
            if (gui.__controllers.hasOwnProperty(i))
            {
                gui.__controllers[i].updateDisplay();
            }
        }
    }

    var refill = false;
    if (magName !== effectController.magnification ||
        minName !== effectController.minification ||
        anisotropy !== effectController.anisotropy)
    {
        magName = effectController.magnification;
        minName = effectController.minification;
        anisotropy = effectController.anisotropy;
        refill = true;

        if (effectController.magnification === 'nearest')
        {
            magVal = THREE.NearestFilter;
        } else
        {
            magVal = THREE.LinearFilter;
        }

        if (effectController.minification === 'nearest')
        {
            minVal = THREE.NearestFilter;
        } else if (effectController.minification === 'linear')
        {
            minVal = THREE.LinearFilter;
        } else
        {
            minVal = THREE.LinearMipMapLinearFilter;
        }

        setFilters();
    }

    if (refill ||
        mtlName !== effectController.mtlName)
    {
        mtlName = effectController.mtlName;
        fillScene();
    }

    for (var name in texture)
    {
        if (texture.hasOwnProperty(name))
        {
            //texture[name].offset.set( effectController.offset, effectController.offset );
            texture[name].repeat.set(effectController.repeat, effectController.repeat);
        }
    }

    camera.fov = effectController.fov;
    camera.updateProjectionMatrix();
    renderer.render(scene, camera);
}

function resetGui()
{
    effectController.magnification = 'linear';
    effectController.minification = 'nearest';
    effectController.anisotropy = 1;
    effectController.fov = 40;
    effectController.repeat = 3;

    effectController.mtlName = 'checker 32x32';

    effectController.reset = false;
}

function setupGui()
{
    effectController = {
        // Actually, resetGui sets the values, use that for defaults
        magnification: 'linear',
        minification: 'nearest',
        anisotropy: 1,
        fov: 40,
        repeat: 3,

        mtlName: 'checker 32x32',

        reset: false
    };

    resetGui();

    gui = new dat.GUI();
    gui.add(effectController, "magnification", ['nearest', 'linear']).name("magnification");
    gui.add(effectController, "minification", ['nearest', 'linear']).name("minification");
    var anisoCount = 1;
    var anisoList = [];
    while (anisoCount <= renderer.getMaxAnisotropy())
    {
        anisoList.push(anisoCount);
        anisoCount *= 2;
    }
    //gui.add( effectController, "anisotropy", anisoList ).name("anisotropy");
    gui.add(effectController, "repeat", 0.1, 10.0).name("texture repeat");
    gui.add(effectController, "fov", 5, 150).name("field of view");
    gui.add(effectController, "mtlName", ['checker 1x1 (gray)', 'checker 2x2', 'checker 4x4', 'checker 8x8', 'checker 16x16', 'checker 32x32', 'checker 64x64', 'checker 128x128', 'checker 256x256', 'checker 512x512', 'crate', 'grid', 'water', 'concrete', 'letterR']).name("texture image");
    gui.add(effectController, "reset").name("reset");
}

try
{
    initRenderer();
    setupGui();
    init();
    animate();
} catch (e)
{
    alert(e);
}
