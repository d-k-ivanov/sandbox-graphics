
var camera, scene, renderer, mouse, stats, geometry, shaderMaterial, mesh, clock;
var mesh, center;
var isLoop;
var mesh1, mesh2;

var objLoader;

var targetArr = [];
var particles = [];
var positionAttribute;
var bunnyURL = "https://s3-us-west-2.amazonaws.com/s.cdpn.io/13842/bunny.obj";

function range(min, max)
{
    return min + (max - min) * Math.random();
}

class CustomGeometry extends THREE.BufferGeometry
{
    constructor(geometry, rd = 1, rd2 = 3)
    {
        super()

        var count = geometry.attributes.position.count;
        var timeArray = new Float32Array(count);
        var randomPosArray = new Float32Array(count * 3);

        for (var ii = 0; ii < count; ii += 3)
        {
            var rand = rd2 * Math.random()
            timeArray[ii + 0] = rand;
            timeArray[ii + 1] = rand;
            timeArray[ii + 2] = rand;

            var randomPosX = range(-rd, rd) + center.x;
            var randomPosY = range(-rd, rd) + center.y;
            var randomPosZ = range(-rd, rd) + center.z;
            randomPosArray[3 * ii + 0] = randomPosArray[3 * ii + 3] = randomPosArray[3 * ii + 6] = randomPosX;
            randomPosArray[3 * ii + 1] = randomPosArray[3 * ii + 4] = randomPosArray[3 * ii + 7] = randomPosY;
            randomPosArray[3 * ii + 2] = randomPosArray[3 * ii + 5] = randomPosArray[3 * ii + 8] = randomPosZ;

        }

        this.addAttribute('aTime', new THREE.BufferAttribute(timeArray, 1));
        this.addAttribute("position", geometry.attributes.position);
        this.addAttribute("randomPos", new THREE.BufferAttribute(randomPosArray, 3));

    }
}

var scale = 60;

function init(group)
{
    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 10000);
    camera.position.z = 200;
    // camera.position.y = 20;
    camera.lookAt(new THREE.Vector3())

    scene = new THREE.Scene();

    group.children[0].geometry.computeBoundingBox();
    group.children[0].geometry.computeBoundingSphere();
    console.log(group.children[0].geometry.boundingBox.min);
    console.log(group.children[0].geometry.boundingBox.max);
    center = group.children[0].geometry.boundingBox.max.clone().add(group.children[0].geometry.boundingBox.min).divideScalar(2);

    var mat = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uCenter: { value: center },
            uColor: { value: new THREE.Vector3(0.1, 0.2, 0.4) },
            uAlpha: { value: 0.75 },
        },
        vertexShader: document.getElementById("vertex").textContent,
        fragmentShader: document.getElementById("fragment").textContent,
        transparent: true,
        depthWrite: false,
        side: THREE.DoubleSide,
        blending: THREE.AdditiveBlending,
    })

    var indices = [];

    var geometry = new CustomGeometry(group.children[0].geometry);

    mesh1 = new THREE.Mesh(geometry, mat);
    mesh1.scale.set(scale, scale, scale);
    mesh1.position.set(-center.x * scale, -center.y * scale, -center.z * scale)

    scene.add(mesh1);

    var mat = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uCenter: { value: center },
            uColor: { value: new THREE.Vector3(1.0, 1.0, 1.0) },
            uAlpha: { value: 0.2 },
        },
        vertexShader: document.getElementById("vertex").textContent,
        fragmentShader: document.getElementById("fragment").textContent,
        transparent: true,
        depthWrite: false,

        side: THREE.DoubleSide,
        blending: THREE.AdditiveBlending
    })

    var indices = [];

    var geometry = new CustomGeometry(group.children[0].geometry, 2, 1.8);

    mesh = new THREE.Mesh(geometry, mat);

    var scale2 = scale / 1.5;
    mesh.scale.set(scale2, scale2, scale2);
    mesh.position.set(-center.x * scale2, -center.y * scale2, -center.z * scale2);
    scene.add(mesh);

    var mat = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uCenter: { value: center },
            uColor: { value: new THREE.Vector3(0.3, 0.1, 0.2) },
            uAlpha: { value: 0.75 },
        },
        vertexShader: document.getElementById("vertex").textContent,
        fragmentShader: document.getElementById("fragment").textContent,
        transparent: true,
        depthWrite: false,
        side: THREE.DoubleSide,
        blending: THREE.AdditiveBlending
    })

    var indices = [];

    var geometry = new CustomGeometry(group.children[0].geometry);

    mesh2 = new THREE.Mesh(geometry, mat);
    mesh2.scale.set(scale, scale, scale);
    mesh2.position.set(-center.x * scale, -center.y * scale, -center.z * scale)
    scene.add(mesh2);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(0x000000);

    clock = new THREE.Clock();
    document.body.appendChild(renderer.domElement);
}

var time = 0;
function loop()
{
    var delta = clock.getDelta();
    time += delta;

    mesh1.material.uniforms.uTime.value = time;
    mesh2.material.uniforms.uTime.value = time;
    // positionAttribute.needsUpdate = true;

    camera.position.z = 20 * Math.cos(time / 3);
    camera.position.x = 20 * Math.sin(time / 3);
    camera.lookAt(new THREE.Vector3());

    renderer.render(scene, camera);
}

objLoader = new THREE.OBJLoader();
objLoader.load(bunnyURL, onLoadObj);

function onLoadObj(geo)
{
    init(geo);
    isLoop = true;
    TweenMax.ticker.addEventListener("tick", loop);
}


window.addEventListener("resize", function (ev)
{
    if (!camera) return;
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
