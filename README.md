<h1>🛡️ Infihash</h1>

<p>A custom memory-hard hashing library for Node.js, implemented in C++ and exposed as an npm module.</p>
<p>It allows hashing passwords with a salt embedded in the stored hash, similar to bcrypt/argon2, so you don’t need to store salt separately.</p>

<p><strong>⚠️ Warning:</strong> This is a custom hashing algorithm and has not been cryptographically audited. Use it only for learning, testing, or experimental purposes. Do not use in production for sensitive passwords.</p>

<h2>✨ Features</h2>
<ul>
  <li>🔒 Generate secure hashes for passwords.</li>
  <li>🧂 Salt is automatically generated and embedded in the hash.</li>
  <li>⚙️ Supports customizable rounds and memory usage.</li>
  <li>🖥️ Simple Node.js API (hash and verify).</li>
  <li>💻 Works on Windows, macOS, and Linux with Node.js native addons.</li>
</ul>

<h2>📦 Installation</h2>
<pre><code>npm install infihash</code></pre>

<p>Note: Since this is a native addon, you need node-gyp installed and a C++ build environment:</p>
<ul>
  <li>🪟 Windows → Install Visual Studio with Desktop C++ workload.</li>
  <li>🍎 macOS → Install Xcode command-line tools (xcode-select --install).</li>
  <li>🐧 Linux → Install build-essential and Python 3.</li>
</ul>

<h2>🛠️ Building from Source</h2>
<p>If you want to build manually (e.g., after cloning the repo):</p>
<pre><code># Clean previous builds
Remove-Item -Recurse -Force build

node-gyp configure

npm run build
</code></pre>

<p>After building, the infihash addon will be available to import in Node.js.</p>

<h2>💡 Usage</h2>
<pre><code>const { hash, verify } = require("infihash");

const password = "mypassword";

// Generate hash (optional salt, rounds, and memory)
const storedHash = hash(password, "", 4, 1024);

console.log("Stored hash (salt:hash):", storedHash);

// Verify correct password
const isCorrect = verify(password, storedHash, 4, 1024);
console.log("✅ Verification (correct password):", isCorrect); // true

// Verify wrong password
const isWrong = verify("wrongpassword", storedHash, 4, 1024);
console.log("❌ Verification (wrong password):", isWrong); // false
</code></pre>

<h2>🧩 API</h2>

<h3>hash(password: string, salt?: string, rounds?: number, memKB?: number): string</h3>
<table border="1" width="100%">
  <tr>
    <th>Parameter</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>password</td>
    <td>🔑 Plain text password (required)</td>
  </tr>
  <tr>
    <td>salt</td>
    <td>🧂 Optional custom salt (default: random 16 characters)</td>
  </tr>
  <tr>
    <td>rounds</td>
    <td>🔄 Number of hashing rounds (default: 3)</td>
  </tr>
  <tr>
    <td>memKB</td>
    <td>💾 Memory in KB to use (default: 1024)</td>
  </tr>
  <tr>
    <td>Returns</td>
    <td>🔐 A string in the format <code>salt:hash</code></td>
  </tr>
</table>

<pre><code>const stored = hash("supersecret");
console.log(stored); // "a1b2c3d4e5f6g7h8:8f1e2d3c4b5a6978..."
</code></pre>

<h3>verify(password: string, storedHash: string, rounds?: number, memKB?: number): boolean</h3>
<table border="1" width="100%">
  <tr>
    <th>Parameter</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>password</td>
    <td>🔑 Plain text password (required)</td>
  </tr>
  <tr>
    <td>storedHash</td>
    <td>🗄️ Previously generated hash string (required)</td>
  </tr>
  <tr>
    <td>rounds</td>
    <td>🔄 Number of hashing rounds (default: 3)</td>
  </tr>
  <tr>
    <td>memKB</td>
    <td>💾 Memory in KB used for hashing (default: 1024)</td>
  </tr>
  <tr>
    <td>Returns</td>
    <td>✅ true if the password matches the hash, ❌ false otherwise</td>
  </tr>
</table>

<pre><code>const ok = verify("supersecret", stored);
console.log(ok); // true

const fail = verify("wrongpass", stored);
console.log(fail); // false
</code></pre>

<h2>⚙️ How It Works</h2>
<ul>
  <li>🧂 Generates a random salt if none is provided.</li>
  <li>💪 Runs a memory-hard custom hash function over the password and salt.</li>
  <li>🔄 Supports multiple rounds and configurable memory usage to increase computational effort.</li>
  <li>🔐 Stores salt embedded in the hash as <code>salt:hash</code>, so no need to manage it separately.</li>
</ul>

<h2>🧪 Testing</h2>
<p>You can test the module using <code>test.js</code>:</p>
<pre><code>node test.js
</code></pre>
<p>Example output:</p>
<pre><code>
Password: mypassword
Stored hash (salt:hash): XT2DeB7H7Oz9cwjB:aa8bf1b64419eb25e572b440a2ddf0cdd947bc07322b17ec89b4bc193c6d77ef
Verification (correct password): true
Verification (wrong password): false
</code></pre>

<h2>📝 Notes</h2>
<ul>
  <li>🧂 Salt is automatically stored in the hash string, no need to store separately in DB.</li>
  <li>💾 Customizable memory (<code>memKB</code>) and rounds allow tuning of CPU/memory cost.</li>
  <li>⚠️ Security warning: This is a custom algorithm, not audited or recommended for production. Use standard libraries like bcrypt or argon2 for sensitive applications.</li>
</ul>

<h2>📄 License</h2>
<p>MIT License</p>
