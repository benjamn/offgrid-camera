var camera = require("offgrid-camera");

console.log("width", camera.width());
console.log("height", camera.height());

setTimeout(function() {
  camera.save("saved.tga");
  setTimeout(function() {
    console.log("done");
  }, 5000);
}, 5000);
