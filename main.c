// Snake game made by 6kmg. Made with using the SDL library. Credits to ChatGPT too.
// Note: Changing the speed might break the game and the score counter is an unsigned char, so if u reach 256 it will turn to 0
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include "include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "func.c"

int main(int argc, char *argv[])  
{
    const short UpdateFrameSpeed = 1000 / FPS;

    double cooldown = 0;
    unsigned short i = 0;
    unsigned short count = (FPS / 14) * 6 + 3;
    unsigned short start;
    char title[64];
    unsigned short score = 0;
    char ending[40];
    unsigned short FrameCounter = 0;

    long long FrameRateOutputSpeed = time(0) + 1;

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
    short SnakeLen[MAXSNAKELEN];
    memset(SnakeLen, -100, MAXSNAKELEN * sizeof(short));
    
    // Random circle position
    short circleX = rand() % (WIDTH - 30) + 15;
    short circleY = rand() % (HEIGHT - 30) + 15;

    SDL_Event windowEvent;
    char running = true;
    while(running)
    {
        // The window title with the simpliest fps counter
        FrameCounter++;
        if (time(0) > FrameRateOutputSpeed){
            sprintf(title, "Snake game | FPS: %d | Score: %d", FrameCounter, score);
            SDL_SetWindowTitle(window, title);
            FrameRateOutputSpeed++;
            FrameCounter = 0;
        }
        
        // Render the background
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        // Drawing the snake head
        SDL_SetRenderDrawColor(renderer, HEADCOLOR);
        SDL_RenderFillRect(renderer, &head);

        DrawCircle(renderer, circleX, circleY, 20, CIRCLECOLOR);

        // Loads up the positions until the third tile
        SnakeLen[0] = head.x;
        SnakeLen[1] = head.y;
        
        if(i < FPS / 2){
            SnakeLen[i] = head.x;
            SnakeLen[i + 1] = head.y;
            i+=2;
        }
        // Draws the first 3 tiles and updates the array
        else{ 
            for (int i = 1; i < 4; i++){
                DrawRect(
                    renderer, 
                    SnakeLen[(int)(FPS/14)*i*2], SnakeLen[(int)(FPS/14)*i*2 + 1], 
                    30, 
                    30, 
                    BODYCOLOR
                );
            }
            SnakeLen[0] = head.x;
            SnakeLen[1] = head.y;
            for (short j = count; j > 2; j -= 2){
                SnakeLen[j] = SnakeLen[j - 2];
                SnakeLen[j - 1] = SnakeLen[j - 3];
            }
        }
        int incrementation = (FPS / 14) * 2;
        for(
            start = ((int) (FPS / 14) * 2) * 4 + 2; 
            start < count; 
            start += incrementation
            ){
            DrawRect(
                renderer, 
                SnakeLen[start], 
                SnakeLen[start + 1], 
                30, 30, BODYCOLOR
            );
            if (
                head.x > SnakeLen[start] && 
                head.y > SnakeLen[start + 1] && 
                head.x < SnakeLen[start] + 30 && 
                head.y < SnakeLen[start + 1] + 30
                ){
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
                    if(
                        windowEvent.key.keysym.sym == MOVERIGHT && 
                        movements[3] != locked && 
                        uTime() > cooldown
                        ){
                        movements[0] = false; 
                        movements[1] = false; 
                        movements[2] = locked; 
                        movements[3] = true;
                        cooldown = uTime() + DELAY;
                    }
                    if(
                        windowEvent.key.keysym.sym == MOVELEFT && 
                        movements[2] != locked && 
                        uTime() > cooldown
                        ){
                        movements[0] = false; 
                        movements[1] = false; 
                        movements[2] = true; 
                        movements[3] = locked;
                        cooldown = uTime()+DELAY;
                    }
                    if(
                        windowEvent.key.keysym.sym == MOVEUP && 
                        movements[0] != locked && 
                        uTime() > cooldown
                        ){
                        movements[0] = true; 
                        movements[1] = locked; 
                        movements[2] = false; 
                        movements[3] = false;
                        cooldown = uTime() + DELAY;
                    }
                    if(
                        windowEvent.key.keysym.sym == MOVEDOWN && 
                        movements[1] != locked && 
                        uTime() > cooldown
                        ){
                        movements[0] = locked; 
                        movements[1] = true; 
                        movements[2] = false; 
                        movements[3] = false;
                        cooldown = uTime() + DELAY;
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
        if (
            head.x > circleX - 50 && 
            head.y > circleY - 50 && 
            head.x < circleX + 20 && 
            head.y < circleY + 20
            ){
            circleX = rand() % (WIDTH - 30) + 15;
            circleY = rand() % (HEIGHT - 30) + 15;
            count+=(FPS/14)*2;
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
    sprintf(ending, "Score: %d", score);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Your Score", ending, 0);
    SDL_Quit();

    return 0;
}
