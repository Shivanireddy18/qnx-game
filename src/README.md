# 🚀 **MINI GAME ON QNX**

# 

## Real-Time Space Defensce System using QNX RTOS

## 

### 📌 Overview



This project is a real-time space defense simulation game built using QNX RTOS. It demonstrates how real-time systems handle multiple tasks with deterministic scheduling, multithreading, and efficient synchronization.



#### The system integrates:

🧠 QNX-based backend (C programming)

🌐 Web-based frontend (HTML, CSS, JavaScript)

🔗 Real-time communication using HTTP \& JSON



### 🎯 Objectives



#### Primary Objectives



* Develop a real-time mini game using QNX RTOS
* Implement multithreading for parallel task execution
* Ensure deterministic and predictable behavior
* Perform real-time collision detection and updates
* Enable backend ↔ frontend communication



#### Secondary Objectives



* Improve system performance using parallelism
* Reduce latency in user interactions
* Maintain data consistency using mutex locks
* Build a scalable and modular architecture
* Demonstrate RTOS concepts like scheduling and IPC



### ⚙️ System Architecture



##### 🧩 Backend (QNX RTOS - C)



* Physics Thread → Handles movement
* AI Thread → Controls enemy behavior
* Collision Thread → Detects interactions (highest priority)
* Network Thread → Handles HTTP communication



##### 🎨 Frontend (Web)



* Built using HTML, CSS, JavaScript
* Uses Canvas for rendering
* Displays:
* Radar view
* Timeline
* Thread activity
* 

##### 🔗 Communication



* Socket-based HTTP communication
* Data exchanged in JSON format



### 🔄 Working Flow



1. System initializes variables and mutex
2. Threads are created and scheduled
3. Frontend sends user input (move/shoot)
4. Network thread processes request
5. Parallel execution:
* Physics updates movement
* AI makes decisions
* Collision detects hits

6\. Shared data protected using mutex

7\. Game state updated

8\. JSON response sent to frontend

9\. Frontend renders output

10\. Loop continues in real-time



### 🧠 Thread Priority

|**THREAD**|**PRIORITY LEVEL**|**REASON**|
|-|-|-|
|COLLISION THREAD|HIGHEST|Critical for detecting hits and avoiding failure|
|PHYSICS THREAD|HIGH|Updates movement and game environment|
|AI THREAD|MEDIUM|Handles decision-making and automation|
|NETWORK THREAD|LOW|Handles communication with frontend|



### 📊 Results



##### ❌ Without QNX (General OS)



* Non-deterministic execution
* Delays in collision detection
* Weak priority handling
* Inconsistent gameplay



##### ✅ With QNX RTOS



* Deterministic scheduling
* Priority-based execution
* Real-time guarantees
* Low latency \& stable performance
* Microkernel reliability



### 🔬 Key Features



⚡ Real-time deterministic execution

🧵 Multithreading with synchronization

🔒 Mutex-based data protection

🌐 Live frontend visualization

📡 Backend-frontend communication

🧠 Simulation of defense systems



### 🛠️ Technologies Used



* QNX RTOS
* C Programming
* HTML, CSS, JavaScript
* Socket Programming (HTTP)
* JSON



### 🚀 Future Scope



📡 Sensor integration (Radar, IR, Ultrasonic)

🤖 AI/ML for intelligent threat detection

🛡️ Real-world defense system applications

🌍 Distributed multi-node architecture



### 🏁 Conclusion

This project successfully demonstrates a real-time multi-threaded system using QNX RTOS. By combining backend processing with frontend visualization, it showcases how real-time scheduling, synchronization, and IPC can be applied in interactive systems.



### 👩‍💻 Authors



**TEAM NAME :** KERNEL SPIKERS



**TEAM MEMBERS:**

&#x20;

V.AKSHAYAJYOTHI

S.SHIVANI

