const addon = require('bindings')('infihash');

function hash(password, salt = "default_salt", rounds = 10) {
  return addon.hash(password, salt, rounds);
}

function generateSalt(length = 16) {
  return addon.generateSalt(length);
}

module.exports = { hash, generateSalt };
