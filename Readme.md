# 🔤 Word Shooter (C++ & GLUT)

**Word Shooter** is a 2D bubble-style game built using C++ and GLUT. You shoot letters at a cluster of bubbles, and when those letters form a valid word, they pop and earn you points. The game is time-based, adding pressure to think and aim quickly!

## 🎮 Gameplay Features
- Bubble shooter mechanics with a word-formation twist
- Shoot letters to form real words
- Words pop when matched correctly
- Score increases with every valid word
- Countdown timer to add urgency

## 🧱 Tech Stack
- **Language:** C++
- **Graphics/Input:** GLUT (OpenGL Utility Toolkit)

## 🧠 Concept
Combining the visual fun of Bubble Shooter with the brainpower of word games, this project offers a unique and challenging gameplay experience.

## 📈 Scoring & Difficulty
- Longer words give more points
- Fast word formations give bonus time or score
- Game ends when the timer runs out or bubbles overflow

## 🚀 How to Run
1. Make sure **GLUT** is installed on your system.
2. Compile the code:
   ```bash
   g++ main.cpp -o WordShooter -lglut -lGL -lGLU
