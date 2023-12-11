#include <SDL2/SDL.h>
#include <stdio.h>

#define FILE_NAME "me.ppm"
#define R_MUL 1
#define G_MUL 1
#define B_MUL 1

// ---

int main() {
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
    int y = i / w;

    int r = buffer[0] * R_MUL;
    int g = buffer[1] * G_MUL;
    int b = buffer[2] * B_MUL;

    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderDrawPoint(ren, x, y);
  }

  fclose(img);  

  SDL_RenderPresent(ren);
  SDL_Delay(5e3);

  return 0;
}
