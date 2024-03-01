#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <cmath> // Include cmath for trigonometric functions


using namespace std;

const float DEG2RAD = 3.14159 / 180;


enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };

// Class representing a Brick
class Brick
{
public:
	// Attributes of a brick
	float red, green, blue; // Color of the brick
	float x, y, width, height; // Position and dimensions of the brick
	int hits;  // New attribute for hit counter
	BRICKTYPE brick_type; // Type of the brick (Reflective or Destructible)
	ONOFF onoff; // On/Off state of the brick

	// Constructor to initialize the brick with specified parameters
	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb, int initialHits)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON; // Initializing the brick as ON by default
		hits = initialHits;
	};

	// Method to draw the brick
	void drawBrick()
	{
		// Check if the brick is in the ON state
		if (onoff == ON)
		{
			// Calculate half width for convenience
			double halfWidth = width / 2;

			// Set the color of the brick
			glColor3d(red, green, blue);

			// Draw the brick as a filled polygon
			glBegin(GL_POLYGON);

			// Specify the vertices of the brick
			glVertex2d(x + halfWidth, y + halfWidth);
			glVertex2d(x + halfWidth, y - halfWidth);
			glVertex2d(x - halfWidth, y - halfWidth);
			glVertex2d(x - halfWidth, y + halfWidth);

			// End drawing
			glEnd();
		}
	}
};

// Class representing a Paddle in the graphics world
class Paddle {
public:
	// Attributes of a paddle
	float x;         // X-coordinate of the paddle
	float y;         // Y-coordinate of the paddle (constant at -1)
	float width;     // Width of the paddle
	float height;    // Height of the paddle
	float speed;     // Speed of the paddle movement

	// Constructor to initialize the paddle with specified parameters
	Paddle(float startX, float paddleWidth, float paddleHeight, float paddleSpeed)
		: x(startX), y(-1.0f), width(paddleWidth), height(paddleHeight), speed(paddleSpeed) {}

	// Method to draw the paddle using OpenGL
	void DrawPaddle() {
		// Set color for the paddle (white in this case, but you can customize it)
		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw the paddle as a filled polygon using GL_POLYGON
		glBegin(GL_POLYGON);
		glVertex2f(x - width / 2, y);
		glVertex2f(x + width / 2, y);
		glVertex2f(x + width / 2, y + height);
		glVertex2f(x - width / 2, y + height);
		glEnd();
	}
};


// Class representing
class Circle
{
public:
	// Attributes of a circle
	float red, green, blue; // Color of the circle
	float radius; // Radius of the circle
	float x, y; // Position of the circle
	float velocityX, velocityY; // Velocity components of the circle
	float speed = 0.03; // Constant speed for the circle
	int direction; // Direction of movement (1=up, 2=right, 3=down, 4=left, 5=up right, 6=up left, 7=down right, 8=down left)

	// Constructor to initialize the circle with specified parameters
	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
		: x(xx), y(yy), radius(rr), direction(dir), red(r), green(g), blue(b)
	{
		// Initialize attributes
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
		
		// Initialize velocity components with random values
		velocityX = static_cast<float>(rand()) / (RAND_MAX / 0.1) - 0.05;
		velocityY = static_cast<float>(rand()) / (RAND_MAX / 0.1) - 0.05;

		// Adjust velocityX if it falls within a specific range
		if (velocityY > 0 && velocityY < 0.03) {
			velocityY += 0.03;
		}

	}
	
	// Method to check collision with a brick and handle accordingly
	void CheckCollision(Brick* brk) {
		// Check if the brick is reflective
		if (brk->brick_type == REFLECTIVE) {
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) &&
				(y > brk->y - brk->width && y <= brk->y + brk->width)) {

				// Reflect the velocity based on the surface normal
				float normalX = brk->x - x;
				float normalY = brk->y - y;
				float magnitude = std::sqrt(normalX * normalX + normalY * normalY);

				normalX /= magnitude;
				normalY /= magnitude;

				// Calculate dot product and reflect the velocity
				float dotProduct = velocityX * normalX + velocityY * normalY;

				velocityX = velocityX - 2.0f * dotProduct * normalX;
				velocityY = velocityY - 2.0f * dotProduct * normalY;

				// Introduce friction to slow down the circle
				velocityX *= 0.98;
				velocityY *= 0.98;


				// If the circle hits a reflective brick, the circle will disappear
				radius = 0.0;
				velocityX = 0.0;
				velocityY = 0.0;
			}
		}
		// Check if the brick is destructible
		else if (brk->brick_type == DESTRUCTABLE) {
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) &&
				(y > brk->y - brk->width && y <= brk->y + brk->width)) {

				// Decrease hit counter
				brk->hits--;

				// Check if hits are exhausted, mark brick as OFF
				if (brk->hits <= 0) {
					brk->onoff = OFF;
				} 
			}
		}
	}

	// Method to check collision with another circle and handle accordingly
	void CheckCircleCollision(Circle* other) {
		if (other != this) {
			if (isCollidingWithCircle(other)) {
				if (red == other->red && green == other->green && blue == other->blue) {
					// Circles of the same color combine to become one larger circle
					radius += other->radius;

					// Assuming the larger circle takes the velocity of the smaller circle
					if (other->radius > radius) {
						velocityX += other->velocityX;
						velocityY += other->velocityY;
					}
					else {
						// Remove only the collided circle from the world
						other->radius = 0.0; // Set radius to 0 to mark for removal
					}
				}

				// Remove both circles from the world if either of them stops
				if (velocityX == 0 && velocityY == 0) {
					radius = 0.0; // Set radius to 0 to mark for removal
					other->radius = 0.0;
				}
			}
		}
	}


	// Method to check if colliding with another circle
	bool isCollidingWithCircle(const Circle* other) const {
		float distance = std::sqrt(std::pow(other->x - x, 2) + std::pow(other->y - y, 2));
		float minDistance = radius + other->radius;

		return distance < minDistance;
	}

	// Method to move the circle one step based on its velocity
	void MoveOneStep() {
		// Update position based on velocity
		x += velocityX;
		y += velocityY;
	}
	
	// Method to draw the circle
	void DrawCircle()
	{
		// Set color and draw the circle using GL_POLYGON
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		glEnd();
	}
};

void processInput(GLFWwindow* window, Paddle& paddle);
vector<Circle> world;

int main(void) {
	// Initialize the random seed using the current time
	srand(time(NULL));

	// Initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Set GLFW window hints for context version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create a GLFW window
	const int WINDOW_WIDTH = 480;
	const int WINDOW_HEIGHT = 480;
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Collisions 2D Animation ", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Create a paddle object
	// Adjust these parameters for desired starting position, size, and movement speed
	float startX = 0.0f;
	float paddleWidth = 0.40f;
	float paddleHeight = 0.05f;
	float paddleSpeed = 0.03f;
	Paddle paddle(startX, paddleWidth, paddleHeight, paddleSpeed);

	// Create a vector to store bricks
	const int NUM_BRICKS = 10;
	vector<Brick> bricks;

	// Generate random bricks with different types, sizes, colors, and non-overlapping positions
	for (int i = 0; i < NUM_BRICKS; ++i) {
		// Randomly choose between REFLECTIVE and DESTRUCTABLE brick types
		BRICKTYPE randomType = static_cast<BRICKTYPE>(rand() % 2);

		// Random width between 0.15 and 0.25
		float randomWidth = static_cast<float>(rand() % 11 + 15) / 100.0f;

		float randomX, randomY;
		bool overlap;

		do {
			// Generate new random positions until a non-overlapping position is found
			randomX = 2.0f * static_cast<float>(rand() % (WINDOW_WIDTH - static_cast<int>(randomWidth * WINDOW_WIDTH))) / static_cast<float>(WINDOW_WIDTH) - 1.0f;  // Random x position between -1.0 and 1.0, inside the window
			randomY = 2.0f * static_cast<float>(rand() % (WINDOW_HEIGHT - static_cast<int>(randomWidth * WINDOW_HEIGHT))) / static_cast<float>(WINDOW_HEIGHT) - 0.75f;  // Random y position between -0.75 and 0.75, inside the window

			overlap = false;

			for (const auto& existingBrick : bricks) {
				float distance = std::sqrt(std::pow(existingBrick.x - randomX, 2) + std::pow(existingBrick.y - randomY, 2));
				float minDistance = (existingBrick.width + randomWidth) / 2.0f;

				if (distance < minDistance) {
					overlap = true;
					break;
				}
			}
		} while (overlap);

		float randomRed = static_cast<float>(rand()) / RAND_MAX;  // Random red component between 0 and 1
		float randomGreen = static_cast<float>(rand()) / RAND_MAX;  // Random green component between 0 and 1
		float randomBlue = static_cast<float>(rand()) / RAND_MAX;  // Random blue component between 0 and 1
		int randomHits = rand() % 3 + 1;
		Brick newBrick(randomType, randomX, randomY, randomWidth, randomRed, randomGreen, randomBlue, randomHits);
		bricks.push_back(newBrick);
	}

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Set up the view and clear the screen
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		// Process user input for the paddle
		processInput(window, paddle);

		// Draw the paddle (using paddle.x after it's updated)
		paddle.DrawPaddle();

		// Movement and collision handling for circles
		for (int i = 0; i < world.size(); i++) {
			// Check for collisions with bricks
			for (int j = 0; j < bricks.size(); j++) {
				world[i].CheckCollision(&bricks[j]);
			}

			// Check for collisions between circles
			for (int j = 0; j < world.size(); j++) {
				if (i != j) {
					world[i].CheckCircleCollision(&world[j]);
				}
			}

			// Move and draw the circle
			world[i].MoveOneStep();
			world[i].DrawCircle();
		}

		// Draw bricks
		for (int i = 0; i < bricks.size(); i++) {
			bricks[i].drawBrick();
		}

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup GLFW
	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window, Paddle& paddle) {
	// Check for escape key to close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// Calculate allowed movement range based on paddle size
	float minX = -1.0f + paddle.width / 2.0f;
	float maxX = 1.0f - paddle.width / 2.0f;

	// Handle left and right movement, ensuring it stays within bounds
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && paddle.x > minX) {
		paddle.x -= 0.03f; // Change to float for consistency
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && paddle.x < maxX) {
		paddle.x += 0.03f;
	}
	

	// Handle space key to create a circle using paddle position
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		float r, g, b;
		r = rand() / 10000.0f; // Cast to float for consistent data type
		g = rand() / 10000.0f;
		b = rand() / 10000.0f;

		Circle B(paddle.x, -1.05, 02, 2, 0.05, r, g, b);
		world.push_back(B);
	}
}