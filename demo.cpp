#include "demo.h"

void put_pixel32(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	Uint32 *pixels = reinterpret_cast<uint32_t*>(surface->pixels);
	pixels[(y * surface->w) + x] = pixel;
}

void demo_init()
{

}

void demo_do(SDL_Surface *surface)
{
	for (int y = 0; y<surface->h; y++) for (int x = 0; x<surface->w; x++) put_pixel32(surface, x, y, 255);
}