<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c&logoColor=white" alt="C Language"/>
  <img src="https://img.shields.io/badge/Course-CSE115-green?style=for-the-badge" alt="CSE115"/>
  <img src="https://img.shields.io/badge/Status-In%20Progress-yellow?style=for-the-badge" alt="Status"/>
</p>

# 🏰 Random Maze Generator & Solver

> A console-based maze generator and solver implemented in **C**, using core programming concepts from **CSE115 — Introduction to Programming**.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Maze Representation](#maze-representation)
- [How It Works](#how-it-works)
  - [Maze Generation — Recursive Backtracking](#maze-generation--recursive-backtracking)
  - [Maze Solving — Recursive Backtracking Search](#maze-solving--recursive-backtracking-search)
- [Project Structure](#project-structure)
- [Function Reference](#function-reference)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Course Topics Applied](#course-topics-applied)
- [Constraints & Notes](#constraints--notes)
- [Team — Group 5](#team--group-5)

---

## Overview

A maze is a grid of cells containing walls and paths. This project addresses two fundamental problems:

1. **Generating** a random maze using recursive backtracking.
2. **Solving** the maze using a recursive backtracking search.

All output is rendered as ASCII art in the terminal. Optionally, mazes can be saved to and loaded from text files.

---

## Features

| Feature | Status |
|---|---|
| Random maze generation (fixed or user-defined size) | ✅ Core |
| Console-based ASCII maze display | ✅ Core |
| Solve maze using recursive backtracking search | ✅ Core |
| Path visualization on solved maze | ✅ Core |
| Save maze to file (`fopen` / `fprintf`) | ⭐ Optional |
| Load maze from file (`fopen` / `fgets`) | ⭐ Optional |
| Dynamic grid size (`malloc` / `free`) | ⭐ Optional |

---

## Maze Representation

The maze is stored as a **2D character array** (`char maze[MAXR][MAXC]`). Each cell uses one of the following symbols:

| Symbol | Meaning |
|---|---|
| `#` | Wall |
| ` ` | Open path |
| `S` | Start |
| `E` | End |
| `.` | Solution path (after solving) |

### Example Output

```
###########
#S#   #   #
# # # # # #
# # #   # #
# ### ### #
#     #   #
##### # # #
#   # # #E#
###########
```

---

## How It Works

### Maze Generation — Recursive Backtracking

The generator treats the grid as a set of cells and carves open paths using recursion:

1. Start from an initial cell.
2. Randomly choose an **unvisited neighbor** (randomised using `rand()`).
3. Remove the wall between the current cell and the chosen neighbor ("carve" a path).
4. **Recurse** into the neighbor.
5. When no unvisited neighbors remain, **backtrack** (the recursion naturally returns).

```
Base case    → No unvisited neighbors remain → return.
General case → Choose a neighbor, carve the wall, recurse on the subproblem.
```

### Maze Solving — Recursive Backtracking Search

The solver uses **recursion** to explore valid neighboring cells from `S` toward `E`:

1. From the current cell, mark it as **visited** in a 2D `visited[][]` array.
2. Check if the current cell is `E` — if yes, a path has been found (**base case**).
3. Otherwise, **recursively try each of the 4 directions** (up, down, left, right).
4. If a direction leads to a dead end (wall or already visited), the function **backtracks** automatically by returning and trying the next direction.
5. Once `E` is reached, the successful path is marked with `.` on the maze grid.

```
Base case    → Current cell is 'E' → return success.
Base case    → Cell is a wall, out of bounds, or already visited → return failure.
General case → Mark visited, try 4 directions recursively, backtrack if stuck.
```

This approach uses only **functions, recursion, 2D arrays, conditions, and loops** — all core CSE115 topics.

---

## Project Structure

Everything lives in a **single source file** with function prototypes at the top, keeping compilation simple:

```
MazeSolver/
├── main.c          # All code: generation, solving, menu, utilities
├── README.md
└── mazes/          # (Optional) saved maze files
    └── maze1.txt
```

---

## Function Reference

All functions are defined in `main.c` with prototypes declared at the top of the file.

### Maze Generation

```c
void init_maze(char maze[][MAXC], int rows, int cols);
    // Fills the entire grid with walls '#'.

void carve(int r, int c, char maze[][MAXC], int rows, int cols, int visited[][MAXC]);
    // Recursively carves open paths from cell (r, c).

void generate_maze(char maze[][MAXC], int rows, int cols);
    // Sets up the grid, places S and E, then calls carve().
```

### Maze Solving

```c
int solve_maze(int r, int c, char maze[][MAXC], int rows, int cols, int visited[][MAXC]);
    // Recursively searches from (r, c) toward 'E'.
    // Returns 1 if a path is found, 0 otherwise.
```

### Display & File I/O

```c
void print_maze(char maze[][MAXC], int rows, int cols);
    // Prints the maze to the console.

void save_maze_to_file(char maze[][MAXC], int rows, int cols, const char *filename);
    // Writes the maze to a text file using fopen/fprintf.  (optional)

void load_maze_from_file(char maze[][MAXC], int *rows, int *cols, const char *filename);
    // Reads a maze from a text file using fopen/fgets.     (optional)
```

---

## Getting Started

### Prerequisites

- **GCC** or any standard C compiler.

### Compile

```bash
gcc main.c -o MazeSolver
```

### Run

```bash
./MazeSolver
```

---

## Usage

On launch, the program presents an interactive menu:

```
========================================
      MAZE GENERATOR & SOLVER
========================================
  1. Generate new random maze
  2. Solve maze
  3. Save maze to file
  4. Load maze from file
  0. Exit
========================================
Enter choice:
```

After solving, the path from `S` to `E` is marked with `.` on the maze grid.

---

## Course Topics Applied

This project demonstrates practical application of the following **CSE115** concepts:

| Topic | Where It Appears in the Project |
|---|---|
| **Selection & Loops** (`if`, `switch`, `for`, `while`, `do-while`) | Menu system, grid iteration, direction checking |
| **`break` / `continue`** | Menu loop control, skipping invalid input |
| **Functions** (prototypes, parameters, return values) | Every operation is a separate function with a prototype |
| **Recursion** (base case + recursive case, call stack) | Maze generation (`carve`) and maze solving (`solve_maze`) |
| **Arrays & 2D Arrays** | `maze[][]` grid representation, `visited[][]` tracking |
| **`struct` / `typedef`** | Grouping maze metadata (rows, cols, start/end position) |
| **Pointers & Call-by-Reference** | Passing arrays to functions, returning values via pointers |
| **`rand()`** | Randomising direction order during maze generation |
| **File Handling** (`FILE*`, `fopen`, `fprintf`, `fgets`, `fclose`) | Save / load mazes to/from text files (optional) |
| **Dynamic Memory** (`malloc` / `free`) | Variable-size grid allocation (optional) |

---

## Constraints & Notes

- Maze boundaries are kept as walls (`#`) to prevent out-of-bound array accesses.
- A `visited[][]` array is used in both generation and solving to prevent revisiting cells.
- Every recursive function has a proper **base case** to avoid stack overflow.
- All dynamically allocated memory is freed before program exit.
- The project compiles as a single file (`main.c`) — no separate compilation needed.

---

## Team — Group 5

| ID | Name |
|---|---|
| 2611404042 | Khalid Been Hossain |
| 2613981642 | Sadeed Ahmad Chowdhury |
| 2611171042 | Wamiq Ahnaaf Hasan |
| 2612716042 | Md. Nasirul Islam |

---

<p align="center">
  Made with ❤️ for CSE115 — Group 5
</p>
