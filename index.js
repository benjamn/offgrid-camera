var offgrid = require('./build/Release/offgrid');

setTimeout(function() {
    offgrid.capture(function(image, w, h) {
        console.log(w, h);
        console.log(image(0, 0));
    });
}, 5000);
