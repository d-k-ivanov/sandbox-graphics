
var image, objLoader, bunnyGeo, particle, clock;
var camera, scene, renderer;
var imgSrc = "https://s3-us-west-2.amazonaws.com/s.cdpn.io/13842/bunny.png"
var texture;
var datas = [];
var targetArr = [];
var MAX = 300000;

objLoader = new THREE.OBJLoader();
objLoader.load("https://s3-us-west-2.amazonaws.com/s.cdpn.io/13842/bunny.obj", onLoadObj);

function range(min, max)
{
    return min + (max - min) * Math.random();
}

class CustomParticle extends THREE.BufferGeometry
{
    constructor()
    {
        super()
        this.count = MAX;
        var positionArray = new Float32Array(this.count * 3);
        var endPositionArray = new Float32Array(this.count * 3);
        var alphaArray = new Float32Array(this.count);
        var timeArray = new Float32Array(this.count);
        var colorArray = new Float32Array(this.count * 3);

        for (var ii = 0; ii < this.count; ii++)
        {
            var data = datas[parseInt(datas.length * Math.random())];
            var targetPos = targetArr[parseInt(targetArr.length * Math.random())];

            positionArray[ii * 3 + 0] = data.x;
            positionArray[ii * 3 + 1] = -10;
            positionArray[ii * 3 + 2] = data.y;

            endPositionArray[ii * 3 + 0] = targetPos.x + range(-1, 1);
            endPositionArray[ii * 3 + 1] = targetPos.y + range(-1, 1);
            endPositionArray[ii * 3 + 2] = targetPos.z + range(-1, 1);

            timeArray[ii] = 3 * Math.random();

            alphaArray[ii] = Math.random();

            colorArray[ii * 3 + 0] = data.color.r;
            colorArray[ii * 3 + 1] = data.color.g;
            colorArray[ii * 3 + 2] = data.color.b;

        }

        this.addAttribute("position", new THREE.BufferAttribute(positionArray, 3));
        this.addAttribute("aTarget", new THREE.BufferAttribute(endPositionArray, 3));
        this.addAttribute("aTime", new THREE.BufferAttribute(timeArray, 1));
        this.addAttribute("aAlpha", new THREE.BufferAttribute(alphaArray, 1));
        this.addAttribute("aColor", new THREE.BufferAttribute(colorArray, 3));
    }
}

class CustomMat extends THREE.ShaderMaterial
{
    constructor()
    {
        var uniforms = {
            uTime: { type: "f", value: 0 }
        };
        super({
            uniforms: uniforms,
            vertexShader: document.getElementById("vertex").textContent,
            fragmentShader: document.getElementById("fragment").textContent
        });

        this.transparent = true;
        this.depthWrite = false;
        this.blending = THREE.AdditiveBlending;
    }
}

function onLoadObj(group)
{
    var geometry = group.children[0].geometry;
    var positionArr = geometry.attributes.position.array;
    var positionCount = positionArr.length / 3;
    var scale = 120;
    var size = 0.2;

    for (var ii = 0; ii < positionCount; ii++)
    {
        var pos = new THREE.Vector3(positionArr[3 * ii] * scale + 3, positionArr[3 * ii + 1] * scale - 15, positionArr[3 * ii + 2] * scale - 15);

        targetArr.push(pos);
    }

    image = new Image();
    image.onload = onLoadimage;
    image.crossOrigin = "Anonymous";
    image.src = imgSrc;


}


function init()
{
    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 0.1, 10000);
    camera.position.z = 30;
    camera.position.y = 30;
    camera.lookAt(new THREE.Vector3())

    scene = new THREE.Scene();

    particle = new THREE.Points(new CustomParticle(), new CustomMat());
    scene.add(particle);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(0x000000);

    clock = new THREE.Clock();
    document.body.appendChild(renderer.domElement);

    document.addEventListener('mousemove', onDocumentMouseMove, false);

    TweenMax.ticker.addEventListener("tick", loop);
}

function onLoadimage()
{
    var canvas = document.createElement("canvas");
    canvas.width = image.width;
    canvas.height = image.height;
    var ctx = canvas.getContext('2d');

    ctx.drawImage(image, 0, 0);

    var data = ctx.getImageData(0, 0, image.width, image.height);
    var imageData = data.data;
    var scale = 10;

    for (var yy = 0; yy < image.height; yy++)
    {
        for (var xx = 0; xx < image.width; xx++)
        {
            var num = 4 * (image.width * yy + xx) + 3;
            var alpha = imageData[num];
            if (alpha != 0)
            {
                var colorRate = xx / image.width
                var color = new THREE.Color();
                color.setHSL(colorRate, 0.3 + 0.3 * Math.random(), 0.3 + 0.3 * Math.random())
                var data = { x: (xx - image.width / 2) / scale, y: (yy - image.height / 2) / scale, alpha: alpha / 255, color: color }
                datas.push(data);
            }

        }
    }


    init();
}

var theta = 0;
var time = 0;
var mouse = new THREE.Vector2();

function loop()
{
    var delta = clock.getDelta();
    time += delta;

    theta += (mouse.x / 3 - theta) / 10;

    camera.position.z = 30 * Math.cos(theta);
    camera.position.x = 30 * Math.sin(theta);
    camera.lookAt(new THREE.Vector3())



    particle.material.uniforms.uTime.value = time;
    renderer.render(scene, camera);
}



function onDocumentMouseMove(event)
{
    event.preventDefault();

    mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
}

window.addEventListener("resize", function (ev)
{
    if (camera)
    {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
    }

    if (renderer)
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
