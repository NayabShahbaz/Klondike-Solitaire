# ♠️♥️ Klondike Solitaire ♣️♦️

This project is a C++ implementation of the classic Klondike Solitaire card game executed with the help of multiple data structures.

---

## ✨ Features
- 🃏 Playable Klondike Solitaire game in the console
- ♻️ Standard rules and card movements
- 🔀 Shuffle and deal functionality
- 🎮 User input for moves

## 🚀 Getting Started

### 🛠️ Prerequisites
- 💻 C++ compiler (e.g., g++, MSVC)

## 🎲 How to Play
- 📋 Follow on-screen instructions to move cards between tableau, foundation, and stock.
- ⌨️ Enter moves as prompted (`m <source>,<destination>,<number>`, `s` for draw a card from stock, `z` for undo last valid move).
- 🏆 The goal is to move all cards to the foundation piles by suit in ascending order.

## 🗃️ Data Structures Used
- 🟦 **Cards**: Implemented using a doubly linked list (DLL) for efficient movement between piles.
- 📚 **Stock & Waste Pile**: Both use a stack data structure.
- 🏛️ **Foundations**: Stack-based, each foundation pile is managed as a stack.
- 📝 **Command Stack**: Stores valid commands for undo functionality.
- 🔄 **Iterators**: Used to move cards between piles instead of deleting and creating new card objects.
- 🧩 **Command Parsing**: Stack-based parsing for user commands.

## 📁 File Structure
- `Klondike Solitaire.cpp`: Main source code file containing the game logic.

## 📜 License
This project is released under the MIT License.

## 👤 Author
- Nayab Shahbaz
