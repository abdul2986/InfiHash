import subprocess
import json
import os
import textwrap

def hash_prompt(prompt, salt="", rounds=3, memKB=1024):
    """
    Hashes a prompt using Node.js InfiHash
    Returns stored hash (salt:hash)
    """
    js_code = textwrap.dedent(f"""
        const {{ hash }} = require('./index');
        const result = hash({json.dumps(prompt)}, {json.dumps(salt)}, {rounds}, {memKB});
        console.log(result);
    """)
    cmd = ["node", "-e", js_code]
    try:
        output = subprocess.check_output(cmd)
        return output.decode().strip()
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Node.js hashing failed: {e.output.decode().strip() if e.output else str(e)}")


CACHE_FILE = "prompt_cache.json"

if os.path.exists(CACHE_FILE):
    with open(CACHE_FILE, "r") as f:
        prompt_cache = json.load(f)
else:
    prompt_cache = {}

def ask_ai(prompt):
    """
    Simulates an AI system
    Caches prompts using InfiHash hash
    """
    # Hash prompt using InfiHash
    prompt_hash = hash_prompt(prompt)
    
    if prompt_hash in prompt_cache:
        return f"[CACHED RESPONSE] {prompt_cache[prompt_hash]}"
    
    # Simulated AI response (dummy AI just reverses text)
    ai_response = f"AI says: {prompt[::-1]}"
    prompt_cache[prompt_hash] = ai_response

    # Save updated cache to file
    with open(CACHE_FILE, "w") as f:
        json.dump(prompt_cache, f)
    
    return ai_response

if __name__ == "__main__":
    print("===== AI PROMPT CACHING DEMO =====\n")
    
    # Ask AI questions
    prompts = ["Explain DSA", "Explain DSA", "What is AI?", "Explain DSA", "What is AI?"]
    for p in prompts:
        print(f"Prompt: {p} -> {ask_ai(p)}")