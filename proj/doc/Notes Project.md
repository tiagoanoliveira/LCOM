# 🧩 Project Name

**Tetr-i/o-s**

---

# 📝 Project Description

We are developing a Tetris-inspired game using an event-driven architecture with **four different I/O devices**.  
The core gameplay mimics the original Tetris mechanics — including rotating, moving, and dropping pieces onto a grid to complete lines.

The game will feature **additional input flexibility** by supporting mouse buttons for complementary actions, such as rotation or hard drop, enhancing the user experience.  
The **visual output** will be handled through a graphical mode for a smoother and more intuitive interface.

---

# 🎮 I/O Devices and Their Purpose

### ⏱️ Timer

Used to control the **automatic descent** of the current piece at fixed intervals, simulating the game's natural pace and increasing difficulty over time.

### ⌨️ Keyboard

Main control device used to **move** the active piece left/right, **rotate** it, or **drop** it instantly to the bottom of the grid.

### 🖱️ Mouse

Used for **secondary actions**:

- **Left click** → rotate piece **left**
- **Right click** → rotate piece **right**
- **Middle click** → perform a **hard drop**

May also be used for interacting with **menus** (optional).

### 🖼️ Graphics

Required to **render the game visually** using pixel-based drawing, including:

- The grid
- The falling pieces
- Score display
- UI elements (start screen, pause, game over)
- Se tivermos problemas com a parte gráfica podemos tentar usar Key Frames, em vez de gerar a janela inteira gera apenas os pixeis que foram modificados na janela

---

# 💡 Possible Extras

- Visual effects when **lines are cleared**
- “Drop” animation with **fade effect**
- Clickable **start menu** using the mouse
- **Theme/skin changes** based on level progression
