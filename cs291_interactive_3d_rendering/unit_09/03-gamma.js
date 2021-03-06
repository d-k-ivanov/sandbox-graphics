"use strict";

// Gamma Correction demo


/*global THREE, requestAnimationFrame, dat, window, document*/

var camera, scene;
var renderer = null;
var cameraControls;
var ec;
var clock = new THREE.Clock();
var light1, light2;
var canvasWidth, canvasHeight;
var geometry;
var material, white_material, gradient_material;
var white_ground, gradient_ground;

function init()
{
    canvasWidth = window.innerWidth;
    canvasHeight = window.innerHeight - 50;
    var canvasRatio = canvasWidth / canvasHeight;

    // CAMERA
    camera = new THREE.PerspectiveCamera(40, canvasRatio, 1, 80000);
    camera.position.set(0, 0, 6000);

    var overlap = 1.3;

    light1 = new THREE.SpotLight();
    light1.color.setRGB(1, 1, 1);
    light1.position.set(-1000 / overlap, 0, 1000);
    light1.exponent = 0;
    light1.target.position.set(-1000 / overlap, 0, 0);
    light1.angle = 0.7;

    light2 = new THREE.SpotLight();
    light2.color.setRGB(1, 1, 1);
    light2.position.set(1000 / overlap, 0, 1000);
    light2.exponent = 0;
    light2.target.position.set(1000 / overlap, 0, 0);
    light2.angle = 0.7;

    // GROUND
    white_material = new THREE.MeshPhongMaterial({ color: 0xFFFFFF, specular: 0x0, side: THREE.DoubleSide });
    gradient_material = new THREE.MeshPhongMaterial({ vertexColors: THREE.VertexColors, specular: 0x0, side: THREE.DoubleSide });

    // material = white_material;

    var sizeX = 4000;
    var sizeY = 2000;

    // geometry = new THREE.PlaneGeometry(10000, 5000);
    geometry = new THREE.BufferGeometry();

    // generate vertices
    var vertices = new Float32Array([
        -sizeX, -sizeY, 0.0,
        sizeX, -sizeY, 0.0,
        sizeX, sizeY, 0.0,
        -sizeX, sizeY, 0.0,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    // generate indexes
    // geometry.faces.push(new THREE.Face4(0, 1, 2, 3));
    var indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));
    geometry.computeVertexNormals();

    white_ground = new THREE.Mesh(geometry, white_material);
    white_ground.visible = true;

    // interestingly enough, cannot use instancing here of the geometry;
    // it appears the material affects how the geometry is stored.
    // geometry = new THREE.PlaneGeometry(10000, 5000);
    geometry = new THREE.BufferGeometry();

    // generate vertices
    vertices = new Float32Array([
        -sizeX, -sizeY, 0.0,
        sizeX, -sizeY, 0.0,
        sizeX, sizeY, 0.0,
        -sizeX, sizeY, 0.0,
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    // generate indexes
    // geometry.faces.push(new THREE.Face4(0, 1, 2, 3));
    indices = new Uint32Array([
        0, 1, 2,
        0, 2, 3,
    ])
    geometry.setIndex(new THREE.BufferAttribute(indices, 1));

    // these actually multiply the color of the material, which is white by default
    // var white = new THREE.Color(0xFFFFFF);
    // var black = new THREE.Color(0x000000);

    var colors = new Uint8Array( [
        0,  0,  0,
        0,  0,  0,
        255,  255,  255,
        255,  255,  255
    ]);

    // geometry.faces[0].vertexColors = [black, black, white, white];
    geometry.setAttribute( 'color', new THREE.BufferAttribute( colors, 3, true) );
    geometry.computeVertexNormals();

    gradient_ground = new THREE.Mesh(geometry, gradient_material);
    gradient_ground.visible = false;

    // EVENTS
    window.addEventListener('resize', onWindowResize, false);

    // GUI
    setupGui();

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(canvasWidth, canvasHeight);
    renderer.setClearColor(new THREE.Color(0xFFFFFF), 1.0);
    renderer.outputEncoding = THREE.sRGBEncoding;

    var container = document.getElementById('container');
    container.appendChild(renderer.domElement);

    cameraControls = new THREE.OrbitAndPanControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);

    fillScene();
}

// EVENT HANDLERS
function onWindowResize()
{
    renderer.setSize(canvasWidth, canvasHeight);
    camera.aspect = canvasWidth / canvasHeight;
    camera.updateProjectionMatrix();
}

function setupGui()
{
    ec = {
        left: 0.7,
        right: 0.2,
        gamma: true,
        gammaIn: false, // we want the input color to be untouched
        gammaOut: true,
        colored: false,
        gradient: false
    };
    light1.intensity = ec.left;
    light2.intensity = ec.right;
    if (ec.colored)
    {
        light1.color.setRGB(1, 0, 0.5);
        light2.color.setRGB(0.5, 0, 1.0);
    } else
    {
        light1.color.setRGB(1, 1, 1);
        light2.color.setRGB(1, 1, 1);
    }

    var gui = new dat.GUI();
    var element = gui.add(ec, "left", 0.0, 1.0).step(0.1).name("Left intensity").onChange(function ()
    {
        light1.intensity = ec.left;
    });

    element = gui.add(ec, "right", 0.0, 1.0).step(0.1).name("Right intensity").onChange(function ()
    {
        light2.intensity = ec.right;
    });

    element = gui.add(ec, "gamma").name("Gamma correction").onChange(function ()
    {
        // if you change gamma input, you need to regenerate the material
        white_material.needsUpdate = true;
        gradient_material.needsUpdate = true;
        //ec.gammaIn = ec.gamma;
        ec.gammaOut = ec.gamma;
        renderer.gammaInput = ec.gammaIn;
        renderer.gammaOutput = ec.gammaOut;
        light1.intensity = ec.left;
        light2.intensity = ec.right;
        if (ec.colored)
        {
            light1.color.setRGB(1, 0, 0.5);
            light2.color.setRGB(0.5, 0, 1.0);
        } else
        {
            light1.color.setRGB(1, 1, 1);
            light2.color.setRGB(1, 1, 1);
        }
    });

    /* allow toggle of in and out */
    element = gui.add( ec, "gammaIn").name("Gamma input").onChange( function() {
        // if you change gamma input, you need to regenerate the material
        white_material.needsUpdate = true;
        gradient_material.needsUpdate = true;
        renderer.gammaInput = ec.gammaIn;
    });

    element = gui.add( ec, "gammaOut").name("Gamma output").onChange( function() {
        // if you change gamma output, you need to regenerate the material
        white_material.needsUpdate = true;
        gradient_material.needsUpdate = true;
        renderer.gammaOutput = ec.gammaOut;
    });

    element = gui.add(ec, "colored").name("Colored lights").onChange(function ()
    {
        if (ec.colored)
        {
            light1.color.setRGB(1, 0, 0.5);
            light2.color.setRGB(0.5, 0, 1.0);
        } else
        {
            light1.color.setRGB(1, 1, 1);
            light2.color.setRGB(1, 1, 1);
        }
    });

    element = gui.add(ec, "gradient").name("Gradient surface").onChange(function ()
    {
        if (ec.gradient)
        {
            white_ground.visible = false;
            gradient_ground.visible = true;
        } else
        {
            white_ground.visible = true;
            gradient_ground.visible = false;
        }
    });
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

function fillScene()
{
    scene = new THREE.Scene();

    // LIGHTS
    scene.add(light1);
    scene.add(light2);
    scene.add(white_ground);
    scene.add(gradient_ground);
}

try
{
    init();
    animate();
} catch (e)
{
    alert(e);
}
