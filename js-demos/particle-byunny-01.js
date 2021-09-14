"use strict";

var camera, cameraControls, scene, renderer, mouse, stats, geometry, shaderMaterial, mesh, clock;
var isLoop;

var objLoader;

var targetArr = [];
var particles = [];
var positionAttribute;
var MAX_COUNT = 60000;
var time = 0;

var utils = {
    range: function (min, max)
    {
        return min + (max - min) * Math.random();
    }
}

class Particle
{
    constructor(x, y, z, index, targetIndex)
    {
        this.x = x;
        this.y = y;
        this.z = z;
        this.prevX = this.x;
        this.prevY = this.y;
        this.prevZ = this.z;
        this.index = index;
        this.targetIndex = targetIndex;
    }

    updateTarget(isDelay)
    {
        this.targetIndex = (this.targetIndex + 1) % targetArr.length;
        var targetPos = targetArr[this.targetIndex];
        TweenMax.to(this, 2, {
            x: targetPos.x,
            y: targetPos.y,
            z: targetPos.z,
            delay: isDelay ? utils.range(0, 2) : 0.0,
            onUpdate: this.onUpdateParticle,
            onUpdateScope: this,
            onComplete: this.updateTarget,
            onCompleteScope: this
        })
    }

    onUpdateParticle()
    {
        positionAttribute.setXYZ(this.index * 2, this.prevX, this.prevY, this.prevZ);
        positionAttribute.setXYZ(this.index * 2 + 1, this.x, this.y, this.z);

        this.prevX = this.x;
        this.prevY = this.y;
        this.prevZ = this.z;
    }
}

function init(group)
{
    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 10000);
    // camera.position.z = 1.2;
    // camera.position.y = 0.5;
    camera.position.z = 20;
    camera.position.y = 20;
    camera.lookAt(new THREE.Vector3())

    scene = new THREE.Scene();

    /**
     mesh = group.children[0];
     console.log(mesh.geometry);
     mesh.material = new THREE.MeshBasicMaterial({wireframe: true, color : 0xffff00})
     mesh.scale.set(10, 10, 10);
     mesh.position.y = -1;

     scene.add(group); */


    // line material
    var material = new THREE.LineBasicMaterial({
        color: new THREE.Color("rgb(" + parseInt(0.2 * 255) + ", " + parseInt(0.3 * 255) + "," + parseInt(0.4 * 255) + ")"),
        blending: THREE.AdditiveBlending,
        depthTest: false,
        transparent: true,
        opacity: 0.6
    });


    var indices = [];

    var positions = new Float32Array(MAX_COUNT * 3);
    var geometry = group.children[0].geometry;
    var positionArr = geometry.attributes.position.array;
    var positionCount = positionArr.length / 3;
    var scale = 150;
    var size = 0.1;

    for (var ii = 0; ii < positionCount; ii++)
    {
        var pos = new THREE.Vector3(positionArr[3 * ii] * scale, positionArr[3 * ii + 1] * scale, positionArr[3 * ii + 2] * scale);

        targetArr.push(pos);
    }

    for (var ii = 0; ii < MAX_COUNT / 2; ii++)
    {
        var index = parseInt(targetArr.length * Math.random());
        var pos = targetArr[index];

        particles[ii] = new Particle(pos.x, pos.y, pos.z, ii, index);

        if (Math.random() < 0.9)
        {
            particles[ii].updateTarget(true);
        }


        positions[6 * ii] = pos.x;
        positions[6 * ii + 1] = pos.y;
        positions[6 * ii + 2] = pos.z;

        positions[6 * ii + 3] = pos.x + utils.range(-size, size);
        positions[6 * ii + 4] = pos.y + utils.range(-size, size);
        positions[6 * ii + 5] = pos.z + utils.range(-size, size);
    }

    for (var ii = 0; ii < MAX_COUNT / 2; ii++)
    {
        indices.push(2 * ii);
        indices.push(2 * ii + 1);
    }

    var geometry = new THREE.BufferGeometry();
    positionAttribute = new THREE.BufferAttribute(positions, 3)
    geometry.addAttribute('position', positionAttribute);
    geometry.setIndex(new THREE.BufferAttribute(new Uint16Array(indices), 1));

    var line = new THREE.LineSegments(geometry, material);
    line.position.set(5, -15, 0);
    scene.add(line);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(0x000000);

    clock = new THREE.Clock();

    document.body.appendChild(renderer.domElement);
    document.addEventListener('mousemove', onDocumentMouseMove, false);

    // CONTROLS
    cameraControls = new THREE.OrbitControls(camera, renderer.domElement);
    cameraControls.target.set(0, 0, 0);
}

function loop()
{
    var delta = clock.getDelta();
    time += delta;

    positionAttribute.needsUpdate = true;

    camera.position.z = 20 * Math.cos(time / 3);
    camera.position.x = 20 * Math.sin(time / 3);
    camera.lookAt(new THREE.Vector3());

    // mesh.rotation.x += 0.01;
    // mesh.rotation.y += 0.02;

    renderer.render(scene, camera);
}

objLoader = new THREE.OBJLoader();
objLoader.load("https://s3-us-west-2.amazonaws.com/s.cdpn.io/13842/bunny.obj", onLoadObj);

function onLoadObj(geo)
{
    init(geo);
    isLoop = true;
    TweenMax.ticker.addEventListener("tick", loop);
}

function onDocumentMouseMove(event)
{
    event.preventDefault();
    if (!mouse) mouse = new THREE.Vector2();

    mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
}

window.addEventListener("resize", function (ev)
{
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize(window.innerWidth, window.innerHeight);
});

window.addEventListener('keydown', function (ev)
{
    switch (ev.which)
    {
        case 27:
            isLoop = !isLoop;
            if (isLoop)
            {
                clock.stop();
                TweenMax.ticker.addEventListener("tick", loop);
            } else
            {
                clock.start();
                TweenMax.ticker.removeEventListener("tick", loop);
            }
            break;
    }
});
