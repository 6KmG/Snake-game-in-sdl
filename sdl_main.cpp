// Snake game made by Champ. Made with using the SDL library. Credits to ChatGPT too.
// Note: Changing the speed might break the game and the score counter is an unsigned char, so if u reach 256 it crashes
// Compile with Powershell
// Debug mode compilation: cd $dir;g++ sdl_main.cpp -Iinclude -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -o sdl_main
// Release mode compilation: g++ sdl_main.cpp -s -mwindows -Iinclude -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -o sdl_main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\SDL.h"
#include <time.h>
#include <math.h>
#include <windows.h>

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUNDCOLOR 101,154,210,255
#define BODYCOLOR 255,255,255,255
#define HEADCOLOR 255,255,255,255
#define CIRCLECOLOR 0,68,130,255
#define SPEED 600
#define FPS 77
#define locked -1   // I set up a third option alongside true and false

const int UpdateFrameSpeed = 1000 / FPS;
const double DELAY = 0.25;

double cooldown = 0;
int i = 0;
short count = (int)(FPS/14)*6+3;
short start;
char title[64];
unsigned char score = 0;
char ending[40];

void DrawRect(SDL_Renderer* renderer, short x, short y, short w, short h, char r, char g, char b, char a) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &rect);
}

// There's no ready to use time function with sub-second detail, so I made one
double uTime(){
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return (double)time(0) + ((double)tp.tv_nsec / 1000000000);
}

// I haven't found a function in the sdl library which draws a filled circle so I copy pasted one from Stackoverflow
void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius, char r, char g, char b, char a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

int main(int argc, char *argv[])  
{
    srand(time(0));
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow
    (
        "Snake game | ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Snake head
    SDL_Rect head;
    head.x = 0;
    head.y = 0;
    head.w = 30;
    head.h = 30;
    char movements[] = {false, false, locked, true};
    
    // I use dynamic allocation since I think I this doesn't fit in the stack
    // I store the cords in 2 elements of list, first is x, second is y and so on
    short* SnakeLen = (short*) malloc(10000 * sizeof(short));
    memset(SnakeLen,-100,10000 * sizeof(short));

    // Random circle position
    short circleX = rand()%(WIDTH - 30) + 15;
    short circleY = rand()%(HEIGHT - 30) + 15;

    SDL_Event windowEvent;
    char running = true;
    while(running)
    {
        // The window title
        uint64_t current_time = SDL_GetPerformanceCounter();
        static uint64_t previous_time = 0;
        uint64_t elapsed_time = current_time - previous_time;
        previous_time = current_time;
        double fps = (double)SDL_GetPerformanceFrequency() / elapsed_time;
        sprintf(title, "Snake game | %f | Score: %d",fps,score);
        SDL_SetWindowTitle(window, title);

        // Render the background
        SDL_SetRenderDrawColor(renderer, BACKGROUNDCOLOR);
        SDL_RenderClear(renderer);

        // Drawing the snake head
        SDL_SetRenderDrawColor(renderer, HEADCOLOR);
        SDL_RenderFillRect(renderer, &head);

        DrawCircle(renderer, circleX, circleY, 20, CIRCLECOLOR);

        // Loads up the positions until the third tile
        SnakeLen[0] = head.x;
        SnakeLen[1] = head.y;
        if(i < FPS/2){
            SnakeLen[i] = head.x;
            SnakeLen[i+1] = head.y;
            i+=2;
        }
        // Draws the first 3 tiles and updates the array
        else{ 
            DrawRect(renderer, SnakeLen[(int)(FPS/14)*2], SnakeLen[(int)(FPS/14)*2+1], 30, 30, BODYCOLOR);
            DrawRect(renderer, SnakeLen[(int)(FPS/14)*4], SnakeLen[(int)(FPS/14)*4+1], 30, 30, BODYCOLOR);
            DrawRect(renderer, SnakeLen[(int)(FPS/14)*6], SnakeLen[(int)(FPS/14)*6+1], 30, 30, BODYCOLOR);
            SnakeLen[0] = head.x;
            SnakeLen[1] = head.y;
            for (short j=count; j>2; j-=2){
                SnakeLen[j] = SnakeLen[j-2];
                SnakeLen[j-1] = SnakeLen[j-3];
            }
        }

        for(start = ((int)(FPS/14)*2)*4+2; start < count; start+=((int)(FPS/14)*2)){
            DrawRect(renderer, SnakeLen[start], SnakeLen[start+1], 30, 30, BODYCOLOR);
            if (head.x > SnakeLen[start] && head.y > SnakeLen[start+1] && head.x < SnakeLen[start]+30 && head.y < SnakeLen[start+1]+30){
                running = false;
            }
        }
        if(SDL_PollEvent(&windowEvent))
        {
            switch(windowEvent.type)
            {
                // Close app on the exit button
                case SDL_QUIT:
                    running = false;
                    break;
                // Keys: each if case checks if any of these key pressed down, checks if the move in
                // opposite direction is forbidden and checks if the cooldown is over
                case SDL_KEYDOWN:
                    if(windowEvent.key.keysym.sym == SDLK_d && movements[3] != locked && uTime()>cooldown){
                        movements[0] = false; movements[1] = false; movements[2] = locked; movements[3] = true;
                        cooldown = uTime()+DELAY;
                    }
                    if(windowEvent.key.keysym.sym == SDLK_a && movements[2] != locked && uTime()>cooldown){
                        movements[0] = false; movements[1] = false; movements[2] = true; movements[3] = locked;
                        cooldown = uTime()+DELAY;
                    }
                    if(windowEvent.key.keysym.sym == SDLK_w && movements[0] != locked && uTime()>cooldown){
                        movements[0] = true; movements[1] = locked; movements[2] = false; movements[3] = false;
                        cooldown = uTime()+DELAY;
                    }
                    if(windowEvent.key.keysym.sym == SDLK_s && movements[1] != locked && uTime()>cooldown){
                        movements[0] = locked; movements[1] = true; movements[2] = false; movements[3] = false;
                        cooldown = uTime()+DELAY;
                    }
                    break;
            }
        }
        // The actual movement happens here with the sipliest deltatime
        if(movements[3] == true){
            head.x += SPEED/FPS;
        }
        else if(movements[2] == true){
            head.x -= SPEED/FPS;
        }
        else if(movements[1] == true){
            head.y += SPEED/FPS;
        }
        else if(movements[0] == true){
            head.y -= SPEED/FPS;
        }
        // On snake eating the circle
        if (head.x > circleX-50 && head.y > circleY-50 && head.x < circleX+20 && head.y < circleY+20){
            circleX = rand()%(WIDTH - 30) + 15;
            circleY = rand()%(HEIGHT - 30) + 15;
            count+=(int)(FPS/14)*2;
            score++;
        }
        // Exit on snake leaving the window
        if (head.x < -30 || head.y < -30 || head.x > WIDTH || head.y > HEIGHT){
            running = false;
        }

        SDL_RenderPresent(renderer);    // This is what actually renders to the window
        SDL_Delay(UpdateFrameSpeed);    // Limits the fps to a given value
    }

    // Closing the program
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    sprintf(ending,"Score: %d",score);
    MessageBox(NULL, ending, "Your Score", MB_OK);
    free(SnakeLen);
    return 0;
}