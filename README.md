# C-_CO_WorkSpace_Founder

A high-performance, interactive Workspace Management & Booking System built from scratch in C++. This terminal-based application features a modern, animated CLI layout designed for professionals and founders to look up, evaluate, and book shared study and work environments seamlessly.

---

## 📺 System Preview (معاينة النظام)

![System Preview](https://github.com/ziadnasar6-lgtm/C-_CO_WorkSpace_Founder/raw/master/fotos/viedo.gif)

> 💡 **Tip for rendering the GIF:** Use an open-source tool like *ScreenToGif* to capture the console startup. Ensure it captures the dynamic Magenta header printing alongside the concurrent typewriter slogan loop (`- Where ideas meet...`) running on the right-hand panel.

---

## 📌 Features

### 💻 Advanced UI & Experience
* **Multi-threaded Background Animation:** Utilizes Windows threads to run a persistent, animated "Founder Insight" widget dynamically alongside core menus without interrupting user interaction.
* **Console Safety & Synchronization:** Built with `CRITICAL_SECTION` hooks to manage mutual exclusion across worker threads, preventing terminal flickering or text overlapping.
* **ANSI Color Palette & Positioning:** Implements explicit screen coordination (`gotoxy`) combined with structured borders, double-box styles, and high-contrast color highlights.

### 🧠 Smart Matching Engine
* **Multifactor Recommendation Algorithm:** Evaluates user inputs across 6 vital vectors (Financial strategy, Sonic environment, Technical priorities, Tactical spacing, Mission nature, and Collaboration style) to score and rank matching venues across Egyptian governorates.
* **Extensive Venue Management:** Filters and displays key information such as ratings, precise district addresses, phone lines, and explicit pricing structures.

### 🔒 User Account & Persistence Engine
* **Full Authentication Flow:** Provides robust Login and Registration modules featuring automated generation of recovery codes (`RECOV-***`).
* **Integrated Loyalty System:** Manages digital credits ("Points") that dynamically update via explicit filesystem reads/writes during booking transitions.
* **Interaction History Pipelines:** Logs and displays distinct persistent states across local storage, covering user-specific Bookings, Comments/Reviews, and curated Favorites arrays.

---

## 📂 File Architecture

The software reads from and persists records into a structured local database using text files:

* `users.txt` - Stores user credentials, points balance, and security metadata.
* `bookings.txt` - Tracks booking ledger histories per account email.
* `comments.txt` - Contains user-submitted workspace reviews and feedback.
* `favorites.txt` - Pairs user profiles with their preferred study locations.
* `/data/*.txt` - Regional datasets segmented by Egyptian Governorates (e.g., Cairo, Alexandria, Qalyubia, etc.).

---

## 🛠️ System Requirements & Build Instructions

### Requirements
* **Operating System:** Windows (Required for native thread loops via `windows.h` and console APIs).
* **Compiler:** Any modern C++ compiler supporting C++11 or higher (GCC/MinGW, MSVC).

### Compilation
Open your terminal or command prompt and execute the following command:

```bash
g++ -O3 main.cpp -o WorkSpaceFounder.exe -lpthread
