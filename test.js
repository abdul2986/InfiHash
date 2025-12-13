const { hash, verify } = require("./index");

const password = "mypassword";
const stored = hash(password, "", 4, 1024);

console.log("Password:", password);
console.log("Stored hash (salt:hash):", stored);

const isCorrect = verify(password, stored, 4, 1024);
console.log("Verification (correct password):", isCorrect);

const isWrong = verify("wrongpassword", stored, 4, 1024);
console.log("Verification (wrong password):", isWrong);
