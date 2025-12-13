<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Infihash - Custom Memory-Hard Hashing Library for Node.js</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.6;
            color: #333;
            background: #f8f9fa;
            margin: 0;
            padding: 0;
        }
        .container {
            max-width: 900px;
            margin: 40px auto;
            background: #ffffff;
            padding: 40px;
            border-radius: 12px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1);
        }
        header {
            text-align: center;
            margin-bottom: 40px;
            border-bottom: 3px solid #007bff;
            padding-bottom: 20px;
        }
        h1 {
            font-size: 2.8rem;
            color: #007bff;
            margin: 0;
        }
        h1 + p {
            font-size: 1.2rem;
            color: #555;
            margin: 10px 0 0;
        }
        .warning {
            background: #fff3cd;
            border: 1px solid #ffeaa7;
            color: #856404;
            padding: 15px 20px;
            border-radius: 8px;
            margin: 30px 0;
            font-weight: 600;
        }
        h2 {
            color: #007bff;
            border-bottom: 2px solid #e9ecef;
            padding-bottom: 10px;
            margin-top: 40px;
        }
        pre {
            background: #2d3748;
            color: #f8f9fa;
            padding: 16px;
            border-radius: 8px;
            overflow-x: auto;
            font-size: 0.95rem;
        }
        code {
            font-family: 'Consolas', 'Monaco', monospace;
        }
        ul {
            padding-left: 20px;
        }
        li {
            margin: 8px 0;
        }
        .note {
            background: #d1ecf1;
            border-left: 5px solid #007bff;
            padding: 12px 16px;
            margin: 20px 0;
            border-radius: 0 6px 6px 0;
            color: #0c5460;
        }
        footer {
            text-align: center;
            margin-top: 60px;
            padding-top: 20px;
            border-top: 1px solid #e9ecef;
            color: #666;
            font-size: 0.9rem;
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>Infihash</h1>
            <p>A custom memory-hard hashing library for Node.js, inspired by bcrypt and Argon2</p>
        </header>

        <p>
            It allows hashing passwords with the <strong>salt embedded directly in the stored hash</strong>, so you don’t need to store the salt separately.
        </p>

        <div class="warning">
            ⚠️ <strong>Warning</strong>: This is a custom hashing algorithm and has <strong>not been cryptographically audited</strong>. 
            Use it for learning, testing, or experimental purposes only. <strong>Do not use in production for sensitive data.</strong>
        </div>

        <h2>Features</h2>
        <ul>
            <li>Generate secure password hashes with customizable rounds and memory usage.</li>
            <li>Salt is automatically generated if not provided.</li>
            <li>Salt is embedded in the stored hash (format: <code>salt:hash</code>), similar to bcrypt.</li>
            <li>Simple and easy-to-use Node.js API for hashing and verification.</li>
        </ul>

        <h2>Installation</h2>
        <pre><code>npm install infihash</code></pre>
        <div class="note">
            <strong>Note:</strong> This package requires Node.js and <code>node-gyp</code> to compile the native C++ addon. 
            Make sure you have the necessary build tools installed on your system.
        </div>

        <h2>Usage</h2>
        <pre><code>const { hash, verify } = require("infihash");

const password = "mypassword";

// Generate hash (salt is optional, rounds and memKB are optional)
const storedHash = hash(password, "", 4, 1024);  // "" means auto-generate salt

console.log("Stored hash (salt:hash):", storedHash);

// Verify correct password
const isCorrect = verify(password, storedHash, 4, 1024);
console.log("Verification (correct password):", isCorrect); // true

// Verify wrong password
const isWrong = verify("wrongpassword", storedHash, 4, 1024);
console.log("Verification (wrong password):", isWrong); // false</code></pre>

        <h2>API</h2>

        <h3><code>hash(password: string, salt?: string, rounds?: number, memKB?: number): string</code></h3>
        <ul>
            <li><strong>password</strong> – The plain text password to hash. <em>(Required)</em></li>
            <li><strong>salt</strong> – Optional custom salt (default: randomly generated 16-character string).</li>
            <li><strong>rounds</strong> – Number of hashing rounds (default: 3).</li>
            <li><strong>memKB</strong> – Memory usage in kilobytes (default: 1024).</li>
            <li><strong>Returns</strong>: A string in the format <code>salt:hash</code>.</li>
        </ul>

        <h3><code>verify(password: string, storedHash: string, rounds?: number, memKB?: number): boolean</code></h3>
        <ul>
            <li><strong>password</strong> – The plain text password to verify. <em>(Required)</em></li>
            <li><strong>storedHash</strong> – The hash string previously generated with <code>hash()</code>. <em>(Required)</em></li>
            <li><strong>rounds</strong> – Number of hashing rounds (default: 3).</li>
            <li><strong>memKB</strong> – Memory used in KB (default: 1024).</li>
            <li><strong>Returns</strong>: <code>true</code> if the password matches, <code>false</code> otherwise.</li>
        </ul>

        <h2>Example</h2>
        <pre><code>const { hash, verify } = require("infihash");

const pwd = "supersecret";
const stored = hash(pwd);  // Uses defaults

console.log(stored); // e.g., "a1b2c3d4e5f6g7h8:8f1e2d3c4b5a6978..."

const ok = verify("supersecret", stored);
console.log(ok); // true

const fail = verify("wrongpass", stored);
console.log(fail); // false</code></pre>

        <h2>Notes</h2>
        <ul>
            <li>The salt is automatically embedded in the hash (<code>salt:hash</code> format), eliminating the need to store it separately.</li>
            <li>Adjust <code>memKB</code> and <code>rounds</code> to increase computational and memory intensity for better resistance against brute-force attacks.</li>
            <li><strong>Security Reminder</strong>: This is a custom implementation, not a battle-tested standard like Argon2 or bcrypt. Use only for educational or non-critical purposes.</li>
        </ul>

        <h2>License</h2>
        <p>MIT License</p>

        <footer>
            © 2025 Infihash – Made for learning and experimentation
        </footer>
    </div>
</body>
</html>