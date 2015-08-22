var offgrid = require('./build/Debug/offgrid');

// Selectively re-export and/or wrap offgrid methods.
exports.save = offgrid.save;
exports.tare = offgrid.tare;
exports.setData = offgrid.setData;
exports.sample = offgrid.sample;
exports.find = offgrid.find;
exports.width = offgrid.width;
exports.height = offgrid.height;
