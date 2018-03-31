#include <iostream>
#include "demo.h"

int main(int argc, char* argv[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "could not create window: " << SDL_GetError() << "\n";
    return 1;
  }
  window = SDL_CreateWindow(
			    "hello_sdl2",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    XRES, YRES,
			    SDL_WINDOW_SHOWN
			    );
  if (window == NULL)
  {
    std::cerr << "could not create window: " << SDL_GetError() << "\n";
    return 1;
  }
  demo_init();
  screenSurface = SDL_GetWindowSurface(window);
  bool quit = false;
  while (!quit)
  {
	  demo_do(screenSurface);
	  SDL_UpdateWindowSurface(window);
	  SDL_Delay(16);
	  SDL_Event event;
	  if (SDL_PollEvent(&event))
	  {
		  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		  {
			  quit = true;
		  }
	  }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
