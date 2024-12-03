#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function to draw a circle (approximated as a set of points)
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

// Function to draw window border
void drawBorder(SDL_Renderer *renderer, int width, int height) {
    // Set border color (dark gray)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    // Draw top, left, right, and bottom borders
    SDL_RenderDrawLine(renderer, 0, 0, width, 0); // Top
    SDL_RenderDrawLine(renderer, 0, 0, 0, height); // Left
    SDL_RenderDrawLine(renderer, width - 1, 0, width - 1, height); // Right
    SDL_RenderDrawLine(renderer, 0, height - 1, width, height - 1); // Bottom
}

// Function to draw ground with brown color and random black texture lines
void drawGround(SDL_Renderer *renderer, int width, int height) {
    // Set the ground to brown color
    for (int y = height - 100; y < height; y++) { // Ground lowered further
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown color (dirt)
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }

    // Add random black lines to simulate ground texture
    srand(time(NULL)); // Seed random number generator
    for (int i = 0; i < 100; i++) { // Add 100 random lines for texture
        int startX = rand() % width; // Random starting point for the line
        int endX = rand() % width;   // Random ending point for the line
        int startY = height - 100 + rand() % 40; // Random vertical position near ground
        int endY = height - 100 + rand() % 40;   // Random vertical position near ground
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for texture
        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    }
}

// SDL entry point (required by SDL2)
int main(int argc, char* argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Take user input in the terminal
    float velocity, angle;
    printf("Enter velocity (m/s): ");
    scanf("%f", &velocity);
    printf("Enter launch angle (degrees): ");
    scanf("%f", &angle);

    // Convert angle to radians
    float angleRad = angle * (M_PI / 180.0f);

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("Projectile Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Main loop
    SDL_Event e;
    int quit = 0;
    float time = 0;
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Calculate projectile position
        float x = velocity * cos(angleRad) * time;
        float y = velocity * sin(angleRad) * time - 0.5 * 9.8 * time * time;

        // If the projectile moves beyond the screen width, stop the simulation
        if (x > 800) {
            quit = 1;
        }

        // Set the background color (light blue)
        SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); // Light blue (sky)
        SDL_RenderClear(renderer);

        // Draw the ground with brown and random texture
        drawGround(renderer, 800, 600);

        // Draw the projectile (a simple red circle)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        drawCircle(renderer, (int)x, 600 - (int)y, 10); // Adjust y to flip it for the screen coordinates

        // Draw window border
        drawBorder(renderer, 800, 600);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Increase time and simulate motion
        time += 0.05;

        // Delay for a short period to control simulation speed
        SDL_Delay(16); // ~60 FPS
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
