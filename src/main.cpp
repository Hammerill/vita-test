#include <SDL.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>

enum {
  SCREEN_WIDTH  = 960,
  SCREEN_HEIGHT = 544
};

SDL_Rect rect = { 
  0, 0, 
  SCREEN_HEIGHT / 2, 
  SCREEN_HEIGHT / 2 
};

void addPos(int* pos, int add, int size, int screenSize)
{
  if (*pos + add < 0)
  {
    *pos = 0;
  }
  else if (*pos + add + size > screenSize)
  {
    *pos = screenSize - size;
  }
  else
  {
    *pos = *pos + add;
  }
}

void setPos(int* x, int* y, int newX, int newY, SDL_Rect rect, int screenWidth, int screenHeight)
{
  if (newX < 0)
  {
    *x = 0;
  }
  else if (newX + rect.w > screenWidth)
  {
    *x = screenWidth - rect.w;
  }
  else 
  {
    *x = newX;
  }

  if (newY < 0)
  {
    *y = 0;
  }
  else if (newY + rect.h > screenHeight)
  {
    *y = screenHeight - rect.h;
  }
  else 
  {
    *y = newY;
  }
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
  SceCtrlData ctrl;

  sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
  sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
  SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];

  const int moveSpeed = 10;
  const int stickDeadZone = 10;
  const int stickCenter = 128;

  bool isRunning = true;
  while (isRunning)
  {
    sceCtrlPeekBufferPositive(0, &ctrl, 1);
    sceTouchPeek(0, &touch[0], 1);

    if (ctrl.buttons & SCE_CTRL_UP)
    {
      addPos(&rect.y, -moveSpeed, rect.h, SCREEN_HEIGHT);
    }
    if (ctrl.buttons & SCE_CTRL_DOWN)
    {
      addPos(&rect.y, moveSpeed, rect.h, SCREEN_HEIGHT);
    }
    if (ctrl.buttons & SCE_CTRL_RIGHT)
    {
      addPos(&rect.x, moveSpeed, rect.w, SCREEN_WIDTH);
    }
    if (ctrl.buttons & SCE_CTRL_LEFT)
    {
      addPos(&rect.x, -moveSpeed, rect.w, SCREEN_WIDTH);
    }

    if (ctrl.lx > stickCenter + stickDeadZone || ctrl.lx < stickCenter - stickDeadZone)
    {
      addPos(&rect.x, (ctrl.lx - stickCenter) / (stickCenter/moveSpeed), rect.w, SCREEN_WIDTH);
    }
    if (ctrl.ly > stickCenter + stickDeadZone || ctrl.ly < stickCenter - stickDeadZone)
    {
      addPos(&rect.y, (ctrl.ly - stickCenter) / (stickCenter/moveSpeed), rect.h, SCREEN_HEIGHT);
    }

    if (touch[0].reportNum > 0)
    {
      setPos(&rect.x, &rect.y, (touch[0].report[0].x/2) - (rect.w/2), (touch[0].report[0].y/2) - (rect.h/2), rect, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  sceKernelExitProcess(0);
  return 0;
}
