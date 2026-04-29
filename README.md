<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c&logoColor=white" alt="C Language"/>
  <img src="https://img.shields.io/badge/Course-CSE115-green?style=for-the-badge" alt="CSE115"/>
  <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge" alt="Status"/>
</p>

# 🏰 Random Maze Generator & Solver

> A console-based maze generator and solver implemented in **C**, using core programming concepts from **CSE115 — Introduction to Programming**.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Maze Representation](#maze-representation)
- [How It Works](#how-it-works)
  - [Maze Generation](#maze-generation)
  - [Maze Solving — Recursive Backtracking](#maze-solving--recursive-backtracking)
- [Project Structure](#project-structure)
- [Function Reference](#function-reference)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Format](#file-format)
- [Course Topics Applied](#course-topics-applied)
- [Constraints & Notes](#constraints--notes)
- [Team — Group 5](#team--group-5)

---

## Overview

A maze is a grid of cells containing walls and paths. This project addresses two fundamental problems:

1. **Generating** a random, fully-connected maze using a recursive backtracking carving algorithm seeded by the default `rand()` sequence.
2. **Solving** the maze using a recursive backtracking search.

All output is rendered as ASCII art in the terminal with ANSI colors. Mazes can also be saved to and loaded from text files.

---

## Features

| Feature | Status |
|---|---|
| Random maze generation via recursive backtracking carving | ✅ Core |
| User-defined maze size (any value; rounded to nearest odd) | ✅ Core |
| Auto-display after generation / load | ✅ Core |
| Solve maze using recursive backtracking | ✅ Core |
| Path visualization on solved maze | ✅ Core |
| Save maze to file (`fopen` / `fprintf`) | ✅ Core |
| Load maze from file (`fopen` / `fscanf`) | ✅ Core |
| Menu-driven interface (`do-while` + `switch`) | ✅ Core |
| Colored terminal output via ANSI escape codes | ✅ Core |

---

## Maze Representation

All maze data is stored in a **global 2D character array** `m[101][101]`, with two global integers `R` and `C` holding the current dimensions. Because the maze is shared by every helper function — `carve`, `gen`, `show`, `solve`, `saveMaze`, `loadMaze` — keeping it global avoids passing the array between calls and makes the recursion in `carve()` and `solve()` simpler to read.

```c
int R, C;           /* current dimensions */
char m[101][101];   /* the maze grid */
```

Each cell uses one of the following symbols (in memory and in save files). When displayed on the terminal, each symbol is rendered with an ANSI color for readability:

| Symbol | Meaning | Terminal Rendering |
|---|---|---|
| `#` | Wall | Bold blue `#` |
| ` ` (space) | Open path | Two blank spaces |
| `S` | Start | Green background, white `S` |
| `E` | End | Red background, white `E` |
| `*` | Solution path (after solving) / visited cell (during recursion) | Bold yellow `*` |

> Colors are emitted as ANSI escape sequences (e.g. `\033[1;34m`). They render on modern terminals including the Windows Terminal and most Unix shells.

### Example (save-file / in-memory view)

> The snippets below use **one character per cell** — the same format written to a text file. On the terminal, `show()` prints each cell **two characters wide** with ANSI colors, so the on-screen output is twice as wide and fully colored.

```
###############
#S            #
# ### ### ### #
#   # # #     #
### # # # ### #
#     #   #   #
# ### ##### # #
#   #     # # #
### ##### # #E#
###############
```

After solving, cells along the discovered route are marked with `*`:

```
###############
#S************#
# ###*###*###*#
#   #*# #*****#
### #*# # ### #
#   # #   #   #
# ### ##### # #
#   #     # # #
### ##### # #E#
###############
```

---

## How It Works

### Maze Generation

The generator builds the maze using a **recursive backtracking carving** algorithm:

1. The user supplies row and column counts. `gen()` rounds each value down to the nearest odd number so that the carving lattice fits cleanly (e.g. `10` becomes `9`).
2. The entire grid is filled with walls (`#`).
3. `carve(1, 1)` is called. At each step it:
   - Marks the current cell as open (` `).
   - Shuffles the four cardinal directions using a **Fisher–Yates shuffle** of an index array.
   - For each shuffled direction it jumps **two cells**; if the destination is still a wall and within bounds, it knocks down the wall in between and recurses into the destination.
4. Because the carving algorithm always produces a fully-connected spanning tree, **every generated maze is guaranteed to be solvable**.
5. After carving, `S` (start) is placed at `(1, 1)` and `E` (end) at `(R-2, C-2)`.

```c
void carve(int r, int c) {
    int d[4] = {0, 1, 2, 3};
    int dr[4] = {-2, 2, 0, 0};
    int dc[4] = {0, 0, -2, 2};
    /* Fisher-Yates shuffle */
    for (i = 3; i > 0; i--) {
        j = rand() % (i + 1);
        t = d[i]; d[i] = d[j]; d[j] = t;
    }
    for (i = 0; i < 4; i++) {
        nr = r + dr[d[i]]; nc = c + dc[d[i]];
        if (nr > 0 && nr < R-1 && nc > 0 && nc < C-1 && m[nr][nc] == '#') {
            m[r + dr[d[i]]/2][c + dc[d[i]]/2] = ' ';
            carve(nr, nc);
        }
    }
}
```

> Because no explicit `srand()` seed is set, `rand()` uses the default deterministic sequence — generating the same size always produces the same layout, which is useful for testing.

### Maze Solving — Recursive Backtracking

The `solve()` function uses recursion to explore valid neighboring cells from `S` toward `E`:

```
Base case → Cell is out of bounds, a wall, or already marked '*' → return failure.
Base case → Current cell is 'E' → return success.
General case → Save old char, mark '*', try 4 directions, backtrack if all fail.
```

Step by step:

1. If the current cell is out of bounds, a wall (`#`), or already marked (`*`), return `0` (failure).
2. If the current cell is `E`, return `1` (success — base case).
3. Save the current cell's character in `old`, then overwrite it with `*` to mark it as visited.
4. **Recursively try all four directions** (down, right, up, left).
5. If any direction succeeds, keep the `*` and return `1`. If the cell was originally `S`, restore it to `S` so the start marker stays visible.
6. If all four directions fail, restore the cell to `old` (**backtracking**) and return `0`.

Before each solve attempt, `main()` runs a loop that resets any leftover `*` marks from a previous run — ensuring the solver always starts clean.

---

## Project Structure

Everything lives in a **single source file**:

```
project/
├── maze.c       # All code: generation, solving, menu, file I/O
└── README.md
```

---

## Function Reference

All functions are defined in `maze.c` and use the global `m[101][101]`, `R`, and `C` directly — no struct or pointer passing required.

```c
void carve(int r, int c);
    // Recursive backtracking carver. Starting from (r, c), carves open
    // paths through the wall grid using a randomised direction order.

void gen(int r, int c);
    // Rounds r/c to odd values, fills the grid with '#', calls carve(1,1),
    // then places 'S' at (1,1) and 'E' at (R-2, C-2).

void show(void);
    // Prints the maze to the console using ANSI color escape codes.

int solve(int r, int c);
    // Recursively searches from (r, c) toward 'E' using backtracking.
    // Marks the successful path with '*'. Returns 1 on success, 0 otherwise.

void saveMaze(void);
    // Prompts for a filename and writes the maze (dimensions + grid) to a
    // text file using fopen / fprintf.

void loadMaze(void);
    // Prompts for a filename, reads the maze back using fopen / fscanf,
    // then auto-displays it with show().
```

---

## Getting Started

### Prerequisites

- **GCC** or any standard C compiler.

### Compile

```bash
gcc maze.c -o maze
```

### Run

```bash
./maze
```

---

## Usage

On launch, the program presents an interactive menu:

```
=== Random Maze Generator & Solver ===
1. Generate Maze
2. Solve Maze
3. Save Maze
4. Load Maze
5. Exit
Enter your choice:
```

- **Generate Maze** — asks for rows and columns, rounds to the nearest odd dimensions, then generates and **automatically displays** the maze.
- **Solve Maze** — clears any leftover path markers, runs the recursive solver, and displays the maze with the solved path marked using `*`. Prints `"No path found."` if unsolvable.
- **Save Maze** — prompts for a filename and writes the current maze to disk.
- **Load Maze** — prompts for a filename, loads the maze, and **automatically displays** it.
- **Exit** — prints `"Exiting..."` and terminates the program.

---

## File Format

Saved mazes use a simple plain-text format:

```
<rows> <cols>
<row 0 characters>
<row 1 characters>
...
```

Example (`maze1.txt`):

```
9 11
###########
#S        #
# ### ### #
#   #   # #
### # ### #
#   #     #
# ####### #
#         #
#########E#
###########
```

The loader reads the size header with `fscanf`, then reads each cell character with `fscanf(fp, "%c", ...)`, consuming the trailing newline between rows to keep the grid aligned.

---

## Course Topics Applied

| Topic | Where It Appears in the Project |
|---|---|
| **Selection & Loops** (`if`, `switch`, `for`, `do-while`) | Menu system, grid initialisation, direction checking, path-clearing loop |
| **Functions** (parameters, return values) | Every operation is a separate named function |
| **Recursion** (base case + recursive case) | `carve()` for maze generation; `solve()` for maze solving |
| **Arrays & 2D Arrays** | Global `m[101][101]` for cell storage; direction arrays in `carve()` |
| **Global Variables** | `m`, `R`, `C` shared across all helper functions and recursive calls |
| **`rand()`** | Randomising direction order during carving (Fisher–Yates shuffle) |
| **File Handling** (`FILE*`, `fopen`, `fprintf`, `fscanf`, `fclose`) | Saving and loading mazes to/from text files |

---

## Constraints & Notes

- Maze borders are always walls (`#`), preventing out-of-bounds array accesses.
- The user may enter any row/column value; `gen()` silently rounds it down to the nearest odd number to fit the carving lattice.
- The internal buffer is `101 × 101`, which comfortably holds any maze up to 99 × 99 cells.
- No explicit `srand()` seed is set — `rand()` produces a deterministic default sequence, so the same input size always generates the same maze layout (useful for testing and grading).
- `solve()` guards against out-of-bounds, wall, and already-visited (`*`) cells, preventing infinite recursion.
- All file pointers are closed via `fclose()` after use.
- Only **`stdio.h`** and **`stdlib.h`** are included — no external libraries needed.
- The terminal must support **ANSI escape codes** for colored output (Windows Terminal, VS Code integrated terminal, and most Unix shells all support this).
- The entire project compiles as a single file (`maze.c`).

---

## Team — Group 5

| ID | Name | Contribution |
|---|---|---|
| 2611404042 | Khalid Been Hossain | Project setup, menu system, `main()` flow |
| 2613981642 | Sadeed Ahmad Chowdhury | Maze generation (`gen`, `carve`) and 2D-array layout |
| 2611171042 | Wamiq Ahnaaf Hasan | Recursive solver (`solve`) and path-clearing logic |
| 2612716042 | Md. Nasirul Islam | File I/O (`saveMaze`, `loadMaze`) and display (`show`) |

---

<p align="center">
  Made with ❤️ for CSE115 — Group 5
</p>
