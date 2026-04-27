# 🔗 Communication Network Simulator (SFML + C++)

A graphical simulation tool to visualize **network reliability and fault tolerance** using **vertex connectivity** and **edge connectivity** concepts from graph theory.

---

## 🚀 Overview

This project simulates a communication network as a **grid-based graph**, where:

* 🔵 Nodes (vertices) are represented as circles
* ➖ Connections (edges) are lines between nodes
* 🔴 Clicking a node simulates **node failure**
* 📊 The system dynamically evaluates how failures affect connectivity

---

## 🎯 Key Features

* ✅ Interactive grid-based network visualization
* ✅ Click to simulate **vertex failure**
* ✅ Real-time update of network structure
* ✅ Displays **minimum edges required to disconnect a node**
* ✅ Demonstrates concepts of:

  * Vertex Connectivity
  * Edge Connectivity
  * Fault Tolerance

---

## 🧠 Concepts Used

### 🔹 Vertex Connectivity

Minimum number of **vertices** whose removal disconnects the graph.

### 🔹 Edge Connectivity

Minimum number of **edges** whose removal disconnects the graph.

### 🔹 Fault Tolerance

Ability of a network to remain connected even after failures.

---

## 🖥️ Tech Stack

* **Language:** C++
* **Graphics Library:** SFML
* **Compiler:** MinGW (MSYS2)
* **IDE:** Visual Studio Code

---

## ⚙️ Installation & Setup

### 🔹 Step 1: Install Dependencies

Install **MSYS2**:
👉 https://www.msys2.org/

Then run:

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-sfml
```

---

### 🔹 Step 2: Add to PATH

Add:

```
C:\msys64\mingw64\bin
```

---

### 🔹 Step 3: Compile & Run

#### 👉 Manual Method

```bash
g++ mesh.cpp -o mesh -lsfml-graphics -lsfml-window -lsfml-system
./mesh
```

---

#### 👉 VS Code

* Press: `Ctrl + Shift + B`
* OR use ▶ Run button

---

## 🎮 How to Use

1. Run the program
2. A grid of nodes will appear
3. Click on any node:

   * 🔴 Node turns red (failed)
   * 📉 Network connectivity updates
4. Observe how many connections are required to isolate nodes

---

## 📸 Demo
### 1. Mesh Topology 

---
<img width="650" alt="Screenshot 2026-04-20 194212" src="https://github.com/user-attachments/assets/bff29514-6f28-4783-875d-dbaaca25d8ac" />
<img width="650"  alt="Screenshot 2026-04-20 194228" src="https://github.com/user-attachments/assets/bced7cd5-8b6d-4d18-a230-1880141b5b40" />

---

### 2. Star Topology
<img width="650"  alt="Screenshot 2026-04-20 200041" src="https://github.com/user-attachments/assets/067583e2-3fff-4907-9267-8fc6979ed9bb" />
<img width="650"  alt="Screenshot 2026-04-20 200105" src="https://github.com/user-attachments/assets/3dbab090-f861-48e9-9a4b-d104d87ed664" />
<img width="650"  alt="Screenshot 2026-04-20 200119" src="https://github.com/user-attachments/assets/031a2a47-f22b-47b0-965f-4004aa4d1e95" />

---

### 3. Ring Topology

<img width="650"  alt="Screenshot 2026-04-20 200646" src="https://github.com/user-attachments/assets/a6140317-2f6c-4146-ad0a-aa9336cd2de6" />
<img width="650"  alt="Screenshot 2026-04-20 200709" src="https://github.com/user-attachments/assets/4ff9a2e9-e5fc-4a51-b593-4a00fb023de5" />






---

## 💡 Example Use Cases

* Understanding network reliability
* Teaching graph theory concepts
* Visualizing fault tolerance in distributed systems

---



## 🤝 Contributing

Pull requests are welcome!
Feel free to open issues for suggestions or bugs.

---

## 📜 License

This project is open-source and free to use.

