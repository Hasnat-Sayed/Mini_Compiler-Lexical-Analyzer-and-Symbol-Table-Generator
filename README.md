# 🧠 Mini Compiler---Lexical Analyzer and Symbol Table Generator

A C++-based lexical analyzer that parses C-style code, detects tokens (keywords, identifiers, operators, etc.), builds a symbol table, and displays results in a user-friendly web interface built with HTML, CSS, and Node.js.

---

## 🚀 Features

- 💻 Backend written in C++ for performance and control
- 🌐 Interactive web interface using HTML + CSS
- 🔎 Real-time tokenization and symbol table generation
- ❗ Highlights invalid identifiers and unknown tokens
- 🔌 Node.js used to bridge frontend and C++ executable

---

## 📂 Project Structure
```bash
compiler-project/ 
├── analyzer.cpp # C++ source code for lexical analyzer
├── analyzer.exe # Compiled analyzer (Windows)
├── server.js # Node.js server to run analyzer backend
├── package.json
├── public/
│   ├── index.html # Web interface
│   ├── styles.css # Token display styles
│   └── script.js # Frontend logic to send code and show results
```

---

## 🛠️ Setup Instructions

### 1. Install Dependencies
```bash  
npm install
```
### 2. Compile the Analyzer
```bash
g++ -std=c++17 analyzer.cpp -o analyzer.exe "-Wl,--subsystem,console"
```
### 3. Run the Server
```bash
node server.js
```
### 4. Open in Browser
Visit: http://localhost:3000

---

## 🧪 Sample Input
```bash
int a = 5;
float b = a + 3.5;
char c = 'x';
int 4abc = 10; // invalid identifier
printf("Value: %d", a);
```

---

## 📸 Screenshots
![compiler1](https://github.com/user-attachments/assets/eb2bfc9d-2356-468e-bf73-0909548df902)

![compiler2](https://github.com/user-attachments/assets/ed7b14ef-f146-49e6-9784-28e7fa5f70b2)

## 🌐 Live Demo (Frontend Only)

You can view the user interface live here:  
🔗 https://mini-compiler-lexical-analyzer-and.vercel.app

> ⚠️ **Note:** This demo is frontend-only.  
> The backend (which runs the C++ lexical analyzer and generates tokens) is **not hosted on Vercel**, so the "Generate Output" button will not work here.

To test the full version with analysis functionality, you can clone and run locally or connect the frontend to a Render-hosted backend.
