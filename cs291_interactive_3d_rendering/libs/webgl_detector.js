var WebGLDetector = {
    canvas:!!window.CanvasRenderingContext2D,
    is_webgl_supported:(function(return_context){
        if (!!window.WebGLRenderingContext) {
            var canvas = document.createElement("canvas"),
                 names = ["webgl2", "webgl", "experimental-webgl", "moz-webgl", "webkit-3d"],
               context = false;
            for(var i = 0; i < names.length; i++) {
                try {
                    context = canvas.getContext(names[i]);
                    if (context && typeof context.getParameter == "function") {
                        // WebGL is enabled
                        if (return_context) {
                            // return WebGL object if the function's argument is present
                            return {name:names[i], gl:context};
                        }
                        // else, return just true
                        return true;
                    }
                } catch(e) {}
            }
            // WebGL is supported, but disabled
            return false;
        }
        // WebGL not supported
        return false;
    })(),
    workers:!!window.Worker,
    fileapi:window.File&&window.FileReader&&window.FileList&&window.Blob,
    errorMessage:function(){
        alert("WebGL not supported! Please visit https://get.webgl.org/ to fix");
    }
};
