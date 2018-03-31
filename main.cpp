#include <iostream>
#include <cstdint>
#define SDL_MAIN_HANDLED
#include <SDL.h>

using std::uint32_t;

#define XRES 800
#define YRES 600

void put_pixel32(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	Uint32 *pixels = reinterpret_cast<uint32_t*>(surface->pixels);
	pixels[ ( y * surface->w ) + x ] = pixel;
}

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
  screenSurface = SDL_GetWindowSurface(window);
  for(int y=0;y<YRES;y++) for(int x=0;x<XRES;x++) put_pixel32(screenSurface, x, y, 255);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(2000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
