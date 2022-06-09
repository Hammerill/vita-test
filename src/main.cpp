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

int addPos(int pos, int add, int size, int screenSize)
{
  if (pos + add < 0)
  {
    return 0;
  }
  else if (pos + add + size > screenSize)
  {
    return screenSize - size;
  }
  else
  {
    return pos + add;
  }
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
  SceCtrlData ctrl;

  const int moveSpeed = 10;
  const int stickDeadZone = 10;
  const int stickCenter = 128;

  bool isRunning = true;
  while (isRunning)
  {
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    if (ctrl.buttons & SCE_CTRL_UP)
    {
      fillRect.y = addPos(fillRect.y, -moveSpeed, fillRect.h, SCREEN_HEIGHT);
    }
    if (ctrl.buttons & SCE_CTRL_DOWN)
    {
      fillRect.y = addPos(fillRect.y, moveSpeed, fillRect.h, SCREEN_HEIGHT);
    }
    if (ctrl.buttons & SCE_CTRL_RIGHT)
    {
      fillRect.x = addPos(fillRect.x, moveSpeed, fillRect.w, SCREEN_WIDTH);
    }
    if (ctrl.buttons & SCE_CTRL_LEFT)
    {
      fillRect.x = addPos(fillRect.x, -moveSpeed, fillRect.w, SCREEN_WIDTH);
    }

    if (ctrl.lx > stickCenter + stickDeadZone || ctrl.lx < stickCenter - stickDeadZone)
    {
      fillRect.x = addPos(fillRect.x, (ctrl.lx - stickCenter) / (stickCenter/moveSpeed), fillRect.w, SCREEN_WIDTH);
    }
    if (ctrl.ly > stickCenter + stickDeadZone || ctrl.ly < stickCenter - stickDeadZone)
    {
      fillRect.y = addPos(fillRect.y, (ctrl.ly - stickCenter) / (stickCenter/moveSpeed), fillRect.h, SCREEN_HEIGHT);
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
