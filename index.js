const addon = require("node-gyp-build")(__dirname);

function hash(password, salt = "", rounds = 3, memKB = 1024) {
  return addon.hash(password, salt, rounds, memKB);
}

function verify(password, storedHash, rounds = 3, memKB = 1024) {
  return addon.verify(password, storedHash, rounds, memKB);
}

module.exports = { hash, verify };
