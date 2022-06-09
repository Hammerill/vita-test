#include <SDL.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

enum {
  SCREEN_WIDTH  = 960,
  SCREEN_HEIGHT = 544
};

SDL_Rect fillRect = { 
  0, 0, 
  SCREEN_HEIGHT / 2, 
  SCREEN_HEIGHT / 2 
};

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SceCtrlData ctrl;

  bool isRunning = true;
  while (isRunning)
  {
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    if (ctrl.buttons & SCE_CTRL_UP && fillRect.y > 0)
    {
      fillRect.y -= 10;
    }
    if (ctrl.buttons & SCE_CTRL_DOWN && fillRect.y < SCREEN_HEIGHT - fillRect.h)
    {
      fillRect.y += 10;
    }
    if (ctrl.buttons & SCE_CTRL_RIGHT && fillRect.x < SCREEN_WIDTH - fillRect.w)
    {
      fillRect.x += 10;
    }
    if (ctrl.buttons & SCE_CTRL_LEFT && fillRect.x > 0)
    {
      fillRect.x -= 10;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  sceKernelExitProcess(0);
  return 0;
}