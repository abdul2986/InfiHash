const { hash } = require('./index');
const hashed = hash('mypassword', '', 4, 1024);
console.log('hash:', hashed); 
