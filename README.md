# Game Project: Mancala-Like Game

## Overview

This project implements a Mancala-like game, where players take turns sowing beans into different holes on the board, capturing beans, and aiming to win by collecting the most beans in their pot. The game supports multiple players, including human players, a bad player (which makes random moves), and a smart player using the minimax algorithm to make intelligent decisions. Project for UCLA CS 32 class. 

## Components

### 1. **Board Class**
- **Description:** 
  - Represents the game board, which is implemented using a doubly linked list for efficient traversal and sowing operations. 
  - Each node represents a hole on the board and contains the hole number, the number of beans, the side of the board (North or South), and pointers to the previous and next holes.
- **Key Features:**
  - Manages sowing operations.
  - Keeps track of beans in play.
  - Implements functionality to move beans to pots, capture beans, and maintain the state of the board throughout the game.
  - Handles edge cases, such as when no beans are left or invalid moves.

### 2. **Player Class**
- **Description:**
  - Base class for different types of players in the game. The `Player` class is abstract and defines virtual methods that are implemented by derived classes.
  - Derived classes include `HumanPlayer`, `BadPlayer`, and `SmartPlayer`, each with their own strategies for making moves.
- **Key Features:**
  - The `HumanPlayer` interacts with the user via the command line to get moves.
  - The `BadPlayer` chooses random moves, ensuring that the chosen hole contains beans.
  - The `SmartPlayer` uses the minimax algorithm to determine the best possible move based on the current state of the game. This player can also account for a time limit when performing deep searches.

### 3. **Game Class**
- **Description:**
  - Coordinates the gameplay between two players.
  - Tracks the game state, including the board, players, and current turn.
- **Key Features:**
  - Handles the main game loop, alternating turns between players.
  - Detects game-over conditions, captures beans, and checks for winner determination.
  - Manages turns when a player lands in their own pot (allowing them to take another turn).

### 4. **SmartPlayer and Minimax Algorithm**
- **Description:**
  - Implements a minimax algorithm with alpha-beta pruning to determine the optimal move.
  - The `SmartPlayer` evaluates the current state of the game by calculating the difference between the player's beans in their pot and the opponent's beans.
- **Key Features:**
  - The algorithm recursively explores all possible moves up to a certain depth (controlled by a time limit or depth limit).
  - It evaluates each move's desirability, maximizing the player's gain and minimizing the opponent's advantages.
  - The `SmartPlayer` performs well against the `BadPlayer` and human players due to its advanced decision-making capabilities.

### 5. **Utilities and Miscellaneous**
- **Description:**
  - Utility functions that assist with game operations, board setup, and other features that enhance the efficiency of game execution.

## Notable Algorithms

### Minimax Algorithm (SmartPlayer)
The SmartPlayer's decision-making process is driven by the minimax algorithm. Here’s an outline of the algorithm:
- If the game is over, return a winning, losing, or tie evaluation.
- If the maximum depth is reached or the time limit is exceeded, return a heuristic evaluation of the current board state.
- For each possible move:
  - Recursively simulate the move and its consequences.
  - For each potential outcome, evaluate the best move for the current player and the worst move for the opponent.
  - Return the move that optimizes the player's strategy while minimizing the opponent's advantages.

### Sowing and Capturing Logic
The `Board` class handles complex sowing operations:
- Players "sow" beans by placing them in subsequent holes in a circular manner.
- If a player lands in their own pot, they can take another turn.
- If the final bean lands in an empty hole on the player's side, they capture the opponent's beans in the opposite hole.

## Key Challenges and Solutions

1. **Minimax Bugs:**
   - Initial attempts at implementing the minimax algorithm caused incorrect move choices (e.g., always picking the rightmost move). This was resolved by using a multimap to track all possible moves and their evaluations, ensuring that the best move was chosen.
   
2. **Efficient Board Representation:**
   - The board is implemented using a circular, doubly linked list to reduce the complexity of sowing operations and ensure efficient navigation through the board during gameplay.

3. **Game State Management:**
   - The `Game` class manages all game states, including alternating between players, detecting when the game ends, and ensuring proper captures and sweeps of beans.

## Test Cases

### Board Class
- **Construction Tests:**
  - Ensure the board initializes correctly with the proper number of holes and beans.
- **Sowing Tests:**
  - Validate that beans are sown correctly, including edge cases like skipping opponent pots.
- **Capture Tests:**
  - Ensure that captures occur correctly when beans land in the player's empty hole.
- **Copy Constructor:**
  - Validate that the copy constructor correctly duplicates the board without linking to the original pointers.

### Player Class
- **Player Name and Interaction:**
  - Test that the name is returned correctly and that human players are interactive while AI players are not.
- **Move Selection:**
  - Validate that players (both bad and smart) choose valid moves, and that smart players make intelligent decisions using minimax.

### Game Class
- **Game Flow:**
  - Test that the game alternates turns correctly, handles multiple turns when landing in the pot, and ends properly when no more beans are left.
- **Winner Detection:**
  - Ensure that the game announces the correct winner or tie at the end.

## Usage

To run the game, compile the provided source files and run the executable. The game can be played between any combination of human players, bad players, or smart players.

## Conclusion

This project provides a complete implementation of a Mancala-like game using C++. It includes advanced AI decision-making via the minimax algorithm, efficient board representation, and flexible player types. The game is tested thoroughly to ensure robustness and fairness in gameplay.