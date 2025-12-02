const { hash, generateSalt } = require("./index");

const salt = generateSalt(12);
const hashed = hash("password123", salt, 8);

console.log("Salt:", salt);
console.log("Hashed:", hashed);
