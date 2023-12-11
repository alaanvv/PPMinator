#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#define FILE_NAME "me.ppm"
#define R_MUL 1
#define G_MUL 1
#define B_MUL 1
#define MIRROR 0
#define UPSIDE_DOWN 0
#define INVERT 0
#define RANDOMIZE 1
#define RANDOM_OFFSET 5

// ---

int main() {
  srand(time(0));

  SDL_Window* win;
  SDL_Renderer* ren;
  
  FILE *img = fopen(FILE_NAME, "r");
  if (!img) { return 1; }
  int w, h;

  fscanf(img, "%*s");
  fscanf(img, "%d", &w);
  fscanf(img, "%d", &h);
  fscanf(img, "%*s");

  int pixelAmount = w * h;
  int buffer[3]; 

  // ---

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(w, h, 0, &win, &ren);

  for (int i = 0; i < pixelAmount; i++) {
    fscanf(img, "%d %d %d", &buffer[0], &buffer[1], &buffer[2]);

    int x = i % w;
    if (MIRROR) { x = w - x; }
    int y = i / w;
    if (UPSIDE_DOWN) { y = h - y; }
    if (RANDOMIZE) {
      x += random() % (RANDOM_OFFSET * 2) - RANDOM_OFFSET;
      y += random() % (RANDOM_OFFSET * 2) - RANDOM_OFFSET;
    }

    int r = buffer[0] * R_MUL;
    int g = buffer[1] * G_MUL;
    int b = buffer[2] * B_MUL;
    if (INVERT) {
      r = 255 - r;
      g = 255 - g;
      b = 255 - b;
    }

    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderDrawPoint(ren, x, y);
  }

  fclose(img);  

  SDL_RenderPresent(ren);
  SDL_Delay(5e3);

  return 0;
}
