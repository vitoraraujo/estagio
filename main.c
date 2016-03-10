#include <SDL2/SDL.h>

int main (void) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    //std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  return 0;
}

