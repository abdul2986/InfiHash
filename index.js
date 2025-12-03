const addon = require('bindings')('infihash');

function hash(password, salt = '', rounds = 3, memKB = 1024) {
  return addon.hash(password, salt, rounds, memKB);
}

module.exports = { hash };
