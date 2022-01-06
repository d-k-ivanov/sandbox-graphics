"use strict";

// Drinking Bird Model exercise                                               //
// Your task is to complete the model for the drinking bird                   //

/*global THREE, Coordinates, $, document, window, dat*/

var camera, scene, renderer;
var cameraControls, effectController;
var clock = new THREE.Clock();
var gridX = false;
var gridY = false;
var gridZ = false;
var axes = false;
var ground = true;

function init()
{
    var canvasWidth = window.innerWidth;
    var canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // RENDERER
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.gammaInput = true;
    renderer.gammaOutput = true;
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xAAAAAA), 1.0);

    // CAMERA
    camera = new THREE.PerspectiveCamera(45, canvasRatio, 1, 40000);
    // CONTROLS
    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);

    camera.position.set(-480, 659, -619);
    cameraControls.target.set(4, 301, 92);

    fillScene();
}

// Supporting frame for the bird - base + legs + feet
function createSupport(wireframe = false)
{
    let cube;
    let cubeMaterial = new THREE.MeshLambertMaterial({ color: 0xF07020, wireframe });
    let legMaterial = new THREE.MeshLambertMaterial({ color: 0xFFFFFF });

    // base
    cube = new THREE.Mesh(new THREE.BoxGeometry(20 + 64 + 110, 4, 2 * 77), cubeMaterial);
    cube.position.x = -45;          // (20+32) - half of width (20+64+110)/2
    cube.position.y = 4 / 2;        // half of height
    cube.position.z = 0;            // centered at origin
    scene.add(cube);

    // left foot
    cube = new THREE.Mesh(new THREE.BoxGeometry(20 + 64 + 110, 52, 6), cubeMaterial);
    cube.position.x = -45;          // (20+32) - half of width (20+64+110)/2
    cube.position.y = 52 / 2;       // half of height
    cube.position.z = 77 + 6 / 2;   // offset 77 + half of depth 6/2
    scene.add(cube);

    // left leg
    cube = new THREE.Mesh(new THREE.BoxGeometry(64, 334 + 52, 6), legMaterial);
    cube.position.x = 0;            // centered on origin along X
    cube.position.y = (334 + 52) / 2;
    cube.position.z = 77 + 6 / 2;   // offset 77 + half of depth 6/2
    scene.add(cube);

    // right foot
    cube = new THREE.Mesh(new THREE.BoxGeometry(20 + 64 + 110, 52, 6), cubeMaterial);
    cube.position.x = -45;          // (20+32) - half of width (20+64+110)/2
    cube.position.y = 52 / 2;       // half of height
    cube.position.z = -77 + 6 / 2;  // offset 77 + half of depth 6/2
    scene.add(cube);

    // right leg
    cube = new THREE.Mesh(new THREE.BoxGeometry(64, 334 + 52, 6), legMaterial);
    cube.position.x = 0;            // centered on origin along X
    cube.position.y = (334 + 52) / 2;
    cube.position.z = -77 + 6 / 2;   // offset 77 + half of depth 6/2
    scene.add(cube);
}

// Body of the bird - body and the connector of body and head
function createBody(wireframe = false)
{
    let sphereMaterial = new THREE.MeshLambertMaterial({ color: 0xA00000, wireframe });
    let cylinderMaterial = new THREE.MeshLambertMaterial({ color: 0x0000D0, wireframe });
    //SphereGeometry( radius, 32, 16 );
    //CylinderGeometry( radiusTop, radiusBottom, height, 32 );

    // body
    let body = new THREE.Mesh(new THREE.SphereGeometry(116 / 2, 32, 16), sphereMaterial);
    body.position.x = 0;
    body.position.y = 160;
    body.position.z = 0;
    scene.add(body);

    // body/head connector
    let bodyHeadConnector = new THREE.Mesh(new THREE.CylinderGeometry(24 / 2, 24 / 2, 390, 16), cylinderMaterial);
    bodyHeadConnector.position.x = 0;
    bodyHeadConnector.position.y = 160 + 390 / 2;
    bodyHeadConnector.position.z = 0;
    scene.add(bodyHeadConnector);

}

// Head of the bird - head + hat
function createHead(wireframe = false)
{
    var sphereMaterial = new THREE.MeshLambertMaterial({ color: 0xA00000, wireframe });
    var cylinderMaterial = new THREE.MeshLambertMaterial({ color: 0x0000D0, wireframe });
    //SphereGeometry( radius, 32, 16 );
    //CylinderGeometry( radiusTop, radiusBottom, height, 32 );

    // head
    let head = new THREE.Mesh(new THREE.SphereGeometry(104 / 2, 32, 16), sphereMaterial);
    head.position.x = 0;
    head.position.y = 160 + 390;
    head.position.z = 0;
    scene.add(head);

    // hat
    let hat = new THREE.Mesh(new THREE.CylinderGeometry(142 / 2, 142 / 2, 10, 16), cylinderMaterial);
    hat.position.x = 0;
    hat.position.y = 160 + 390 + 104 / 2 - 10 / 2;
    hat.position.z = 0;
    scene.add(hat);

    hat = new THREE.Mesh(new THREE.CylinderGeometry(80 / 2, 80 / 2, 70, 16), cylinderMaterial);
    hat.position.x = 0;
    hat.position.y = 160 + 390 + 104 / 2 + 70 / 2;
    hat.position.z = 0;
    scene.add(hat);
}

function createDrinkingBird()
{
    // let wireframe = true
    let wireframe = false
    // MODELS
    // base + legs + feet
    createSupport(wireframe);

    // body + body/head connector
    createBody(wireframe);

    // head + hat
    createHead(wireframe);
}

function fillScene()
{
    // SCENE
    scene = new THREE.Scene();
    scene.fog = new THREE.Fog(0x808080, 3000, 6000);

    // LIGHTS
    var ambientLight = new THREE.AmbientLight(0x222222);
    var light = new THREE.DirectionalLight(0xFFFFFF, 1.0);
    light.position.set(200, 400, 500);

    var light2 = new THREE.DirectionalLight(0xFFFFFF, 1.0);
    light2.position.set(-400, 200, -300);

    scene.add(ambientLight);
    scene.add(light);
    scene.add(light2);

    if (ground)
    {
        Coordinates.drawGround({ size: 1000 });
    }
    if (gridX)
    {
        Coordinates.drawGrid({ size: 1000, scale: 0.01 });
    }
    if (gridY)
    {
        Coordinates.drawGrid({ size: 1000, scale: 0.01, orientation: "y" });
    }
    if (gridZ)
    {
        Coordinates.drawGrid({ size: 1000, scale: 0.01, orientation: "z" });
    }
    if (axes)
    {
        Coordinates.drawAllAxes({ axisLength: 300, axisRadius: 2, axisTess: 50 });
    }
    createDrinkingBird();
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
    if (effectController.newGridX !== gridX ||
        effectController.newGridY !== gridY ||
        effectController.newGridZ !== gridZ ||
        effectController.newGround !== ground ||
        effectController.newAxes !== axes)
    {
        gridX = effectController.newGridX;
        gridY = effectController.newGridY;
        gridZ = effectController.newGridZ;
        ground = effectController.newGround;
        axes = effectController.newAxes;

        fillScene();
    }
    renderer.render(scene, camera);
}

function setupGui()
{
    effectController = {

        newGridX: gridX,
        newGridY: gridY,
        newGridZ: gridZ,
        newGround: ground,
        newAxes: axes
    };

    var gui = new dat.GUI();
    gui.add(effectController, "newGridX").name("Show XZ grid");
    gui.add(effectController, "newGridY").name("Show YZ grid");
    gui.add(effectController, "newGridZ").name("Show XY grid");
    gui.add(effectController, "newGround").name("Show ground");
    gui.add(effectController, "newAxes").name("Show axes");
}

try
{
    init();
    setupGui();
    addToDOM();
    animate();
} catch (e)
{
    alert(e);
}
