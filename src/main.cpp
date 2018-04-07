#include <iostream>
#include <ctime>
#include "main.hpp"
#include "scene_perlin.hpp"
#include "scene_sphere.hpp"

bool quit = false;
int deltaT = 16;
int cnt = 0;
clock_t start;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

void init() {
  scene_sphere_init();
  scene_perlin_init();
  clock_t start = clock();
}
void loop() {
  float elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
  if (elapsed_secs < 10) {
    scene_perlin_do(screenSurface, deltaT, cnt);
  } else {
    scene_sphere_do(screenSurface, deltaT, cnt);
  }
  SDL_UpdateWindowSurface(window);
  SDL_Delay(deltaT);
  SDL_Event event;
  if (SDL_PollEvent(&event))
  {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
    {
      quit = true;
    }
  }
  cnt++;
}

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "could not create window: " << SDL_GetError() << "\n";
    return 1;
  }
  window = SDL_CreateWindow(
			    "Hello Worlds <3",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    XRES, YRES,
			    SDL_WINDOW_SHOWN
			    );
  if (window == NULL) {
    std::cerr << "could not create window: " << SDL_GetError() << "\n";
    return 1;
  }
  init();
  screenSurface = SDL_GetWindowSurface(window);
  #ifdef __EMSCRIPTEN__
    // 0 fps means to use requestAnimationFrame; non-0 means to use setTimeout.
    emscripten_set_main_loop(loop, 0, 1);
  #else
    while (!quit) {
      loop();
      SDL_Delay(16);
    }
  #endif
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
