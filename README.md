# 2D Collision Game

This project is a simple 2D collision game implemented using OpenGL and GLFW. The game involves a paddle, bricks, and circles that collide and interact with each other. The objective is to showcase basic collision detection and response mechanisms in a 2D environment.

## Features

- **Paddle**: Move the paddle left and right using arrow keys to prevent circles from going below the screen.

- **Bricks**: Randomly generated bricks of reflective and destructible types. Reflective bricks bounce back circles, while destructible bricks decrease their hit points upon collision.

- **Circles**: Press the space key to launch circles from the paddle. Circles can collide with each other and with bricks, exhibiting different behaviors.

## How to Play

1. **Launch the Game**:
   - Compile and run the code using an OpenGL-compatible environment.
   - Use arrow keys to move the paddle.
   - Press the space key to create circles.

2. **Interact with Bricks**:
   - Reflective bricks reflect circles upon collision.
   - Destructible bricks decrease hit points upon collision.

3. **Collisions Between Circles**:
   - Circles of the same color combine into a larger circle.

4. **Game Over**:
   - The game continues until the window is closed.

## Dependencies

- [GLFW](https://www.glfw.org/): A multi-platform library for creating windows with OpenGL contexts.

## How to Build and Run

1. Clone the repository:

   ```bash
   git clone https://github.com/hxn6770-2k/2D_Collision_Game.git
