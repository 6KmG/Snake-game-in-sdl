#pragma once

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include "include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#define WIDTH 800
#define HEIGHT 450
#define BACKGROUND_COLOR 101,154,210,255
#define BODYCOLOR 255,255,255,255
#define HEADCOLOR 255,255,255,255
#define CIRCLECOLOR 0,68,130,255
#define SPEED 600
#define FPS 60
#define MAXSNAKELEN 10000   // Don't go above 1 million
#define DELAY 0.125

#define locked 2   // I set up a third option alongside true and false
#define false 0
#define true 1

#define MOVELEFT SDLK_a
#define MOVERIGHT SDLK_d
#define MOVEUP SDLK_w
#define MOVEDOWN SDLK_s

void DrawRect(
    SDL_Renderer* renderer, 
    short x, short y, short w, short h, 
    char r, char g, char b, char a
    ) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

// There's no ready to use time function with sub-second detail, so I made one based on chatGPT's idea
double uTime(){
    struct timespec ts;
    clock_gettime(0, &ts);
    return (double) ts.tv_sec + ((double) ts.tv_nsec / 1000000000);
}

// I haven't found a function in the sdl library which draws 
// a filled circle so I copy pasted one from Stackoverflow
void DrawCircle(
    SDL_Renderer *renderer,     
    short x, short y, short radius, 
    char r, char g, char b, char a
    )
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (short w = 0; w < radius * 2; w++)
    {
        for (short h = 0; h < radius * 2; h++)
        {
            short dx = radius - w;
            short dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}