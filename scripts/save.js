var camera = require("offgrid-camera");
var spawn = require("child_process").spawn;

setTimeout(function() {
  camera.save("saved.tga");
  spawn("convert", ["saved.tga", "saved.jpg"]);
}, 3000);
