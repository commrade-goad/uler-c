#include "raylib-5.0_linux_amd64/include/raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define SIZE 20

struct Coords {
    int x;
    int y;
};

enum Direction { 
    TOP,
    BOTTOM,
    RIGHT,
    LEFT
};

Rectangle gen_rec(struct Coords coords, int w, int h) {
    Rectangle result = {.x = coords.x, .y = coords.y, .width = (float) w, .height = (float) h};
    return result;
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Hello from C");
    SetExitKey('Q');
    SetTargetFPS(60);

    enum Direction snake_dir = RIGHT;
    struct Coords food = { .x = GetRandomValue(0, WIDTH - SIZE), .y = GetRandomValue(0, HEIGHT - SIZE)};
    struct Coords center = { .x = (int) WIDTH / 2, .y = (int) HEIGHT / 2 };
    struct Coords snake = { .x = center.x, .y = center.y };
    struct Coords *snake_body = NULL; 
    int snake_len = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();

        Rectangle snake_rectangle = gen_rec(snake, SIZE, SIZE);
        Rectangle food_rectangle = gen_rec(food, SIZE, SIZE);

        if (snake_len > 0) {
            if (IsKeyDown('W')) if (snake_dir != BOTTOM) snake_dir = TOP;
            if (IsKeyDown('S')) if (snake_dir != TOP) snake_dir = BOTTOM;
            if (IsKeyDown('A')) if (snake_dir != RIGHT) snake_dir = LEFT;
            if (IsKeyDown('D')) if (snake_dir != LEFT) snake_dir = RIGHT;
        } else {
            if (IsKeyDown('W')) snake_dir = TOP;
            if (IsKeyDown('S')) snake_dir = BOTTOM;
            if (IsKeyDown('A')) snake_dir = LEFT;
            if (IsKeyDown('D')) snake_dir = RIGHT;
        }

        // Move the snake
        struct Coords prev_position = snake;

        switch (snake_dir) {
            case TOP:    snake.y -= (int) 60 / SIZE; break;
            case BOTTOM: snake.y += (int) 60 / SIZE; break;
            case LEFT:   snake.x -= (int) 60 / SIZE; break;
            case RIGHT:  snake.x += (int) 60 / SIZE; break;
        }

        // Handle screen wrapping
        if (snake.x >= WIDTH) snake.x = 0;
        if (snake.x < 0) snake.x = WIDTH - SIZE;
        if (snake.y >= HEIGHT) snake.y = 0;
        if (snake.y < 0) snake.y = HEIGHT - SIZE;

        // Check collision with food
        if (CheckCollisionRecs(snake_rectangle, food_rectangle)) {
            snake_len += 3;
            snake_body = realloc(snake_body, sizeof(struct Coords) * snake_len);
            food.x = GetRandomValue(0, WIDTH - SIZE);
            food.y = GetRandomValue(0, HEIGHT - SIZE);
        }

        // Update the snake body
        if (snake_len > 0) {
            for (int i = snake_len - 1; i > 0; i--) {
                snake_body[i] = snake_body[i - 1];

                if (snake_body[i].x == snake.x && snake_body[i].y == snake.y) {
                    CloseWindow();
                    free(snake_body);
                    return 0;
                }
            }
            snake_body[0] = prev_position;
        }

        // Draw the snake body
        for (int i = 0; i < snake_len; i++) {
            DrawRectangle(snake_body[i].x, snake_body[i].y, SIZE, SIZE, GetColor(0xddddddff));
        }

        DrawRectangle(food.x, food.y, SIZE, SIZE, GRAY);
        DrawRectangle(snake.x, snake.y, SIZE, SIZE, RAYWHITE);

        ClearBackground(BLACK);
        EndDrawing();
    }

    free(snake_body);
    CloseWindow();
    return 0;
}

