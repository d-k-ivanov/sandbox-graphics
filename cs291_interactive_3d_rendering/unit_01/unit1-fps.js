"use strict"; // good practice - see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
// FPS demo
// Use the slider to adjust FPS an see how that changes the responsiveness
// of the scene.
/*global THREE, requestAnimationFrame, Stats, dat, window, document */

var container, camera, scene, renderer, stats;
var cameraControls;
var effectController;
var clock = new THREE.Clock();
var teapotSize = 400;
var ambientLight, light, light2;
var teapot;
var newTime = 0, oldTime = 0;

function setupGui()
{
    effectController = {
        fps: 30.0
    };

    var gui = new dat.GUI();
    var element = gui.add(effectController, "fps", 1.0, 60.0).step(1.0);
    element.name("FPS");
}

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
    var canvasHeight = window.innerHeight;
    var canvasRatio = canvasWidth / canvasHeight;

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 1, 80000);
    camera.position.set(-800, 700, 1600);

    // SCENE
    scene = new THREE.Scene();

    // LIGHTS
    ambientLight = new THREE.AmbientLight(0x222222);
    scene.add(ambientLight);

    light = new THREE.DirectionalLight(0xFFFFFF, 0.8);
    light.position.set(320, 390, 700);
    scene.add(light);

    light2 = new THREE.DirectionalLight(0xFFFFFF, 0.5);
    light2.position.set(-720, -190, -300);
    scene.add(light2);

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA, 1.0));

    // renderer.gammaInput = true; // Texture.encoding
    // renderer.outputEncoding = true;
    addToDOM();
    // STATS
    stats = new Stats();
    stats.setMode(1);
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '0px';
    stats.domElement.style.zIndex = 100;
    stats.domElement.style.color = "#aaa";
    stats.domElement.style.background = "transparent";
    // stats.domElement.style.display = "none";
    container.appendChild(stats.domElement);

    // CONTROLS
    cameraControls = new THREE.OrbitControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);

    // MATERIALS
    // Note: setting per pixel off does not affect the specular highlight;
    // it affects only whether the light direction is recalculated each pixel.
    var lambertMaterial = new THREE.MeshLambertMaterial({ color: 0xb00505 });
    lambertMaterial.side = THREE.DoubleSide;

    // to test texturing, uncomment the following four lines
    var path = "";
    // var texture = new THREE.TextureLoader().load( path + '../textures/uv-grid-01.jpg' );
    // var texture = new THREE.TextureLoader().load( path + '../textures/rust-01.jpg' );
    var texture = new THREE.TextureLoader().load(path + '../textures/metall-01.jpg');
    texture.anisotropy = renderer.capabilities.getMaxAnisotropy();
    texture.outputEncoding = true;
    lambertMaterial = new THREE.MeshLambertMaterial({ map: texture });

    teapot = new THREE.Mesh(
        new THREE.TeapotGeometry(teapotSize, 8, true, true, true, true),
        lambertMaterial);

    scene.add(teapot);

    const backgroundTexture = new THREE.TextureLoader().load(path + '../textures/stars-01.jpg', function (texture)
    {
        var img = texture.image;
        var imageAspect = img.width / img.height;
        const factor = imageAspect / canvasRatio;
        scene.background.offset.x = factor < 1 ? (1 - 1 / factor) / 2 : 0;
        scene.background.repeat.x = factor < 1 ? 1 / factor : 1;
        scene.background.offset.y = factor < 1 ? 0 : (1 - factor) / 2;
        scene.background.repeat.y = factor < 1 ? 1 : factor;
    });
    scene.background = backgroundTexture;
    backgroundTexture.wrapS = THREE.RepeatWrapping;
    backgroundTexture.wrapT = THREE.RepeatWrapping;
    // scene.background = new THREE.Color(0x808080);

    // GUI
    setupGui();
}

function render()
{
    var delta = clock.getDelta();
    cameraControls.update(delta);
    newTime += delta;

    // fudge factor: 0.95 correlates closer to true frame rate numbers;
    // basically, there's some friction as far as timing goes, and this adjusts for it.
    var frameTime = 0.95 / effectController.fps;
    if (effectController.fps > 59.9)
    {
        // At 60 FPS, simply go as fast as possible;
        // Not doing so can force a frame time that is less than 60 FPS.
        frameTime = 0;
    }

    if (newTime > oldTime + frameTime)
    {
        oldTime = newTime;
        renderer.render(scene, camera);
        stats.update();
    }
}

function animate()
{
    requestAnimationFrame(animate);
    render();
}

init();
animate();
