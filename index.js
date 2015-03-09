var offgrid = require('./build/Release/offgrid');

// Selectively re-export and/or wrap offgrid methods.
exports.capture = offgrid.capture;
exports.tare = offgrid.tare;
exports.find = offgrid.find;
exports.switch = offgrid.switch;
