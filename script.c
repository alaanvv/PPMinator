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
#define RANDOMIZE_X 0
#define RANDOMIZE_Y 1
#define RANDOM_OFFSET 50
#define CLEAR 0
#define ROTATE 1

// ---

SDL_Window* win;
SDL_Renderer* ren;

FILE *img;
FILE *bak;
int w, h;
int pixelAmount;
int buffer[3]; 
int init();
void finish();

// ---

int main() {
  init();

  int a = 0;

  SDL_Event e;
  int running = 1;
  while (running) {
    while(SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) { running = 0; } }
    if (CLEAR) { SDL_RenderClear(ren); }
    bak = img;

    for (int i = 0; i < pixelAmount; i++) {
      fscanf(img, "%d %d %d", &buffer[0], &buffer[1], &buffer[2]);

      int x = i % w;
      if (MIRROR) { x = w - x; }

      int y = i / w;
      if (UPSIDE_DOWN) { y = h - y; }

      if (RANDOMIZE_X) { x += random() % (RANDOM_OFFSET * 2) - RANDOM_OFFSET; }
      if (RANDOMIZE_Y) { y += random() % (RANDOM_OFFSET * 2) - RANDOM_OFFSET; }

      int neww = w * cos(a * 3.1415 / 180);
      x = neww * (float) x / w;

      int newh = h + (h * 0.5 * (neww / 2 - x) / (neww / 2) * sin(a * 3.1415 / 180));
      y = ((h - newh) / 2) + newh * (float) y / h;

      int r = buffer[0] * R_MUL;
      int g = buffer[1] * G_MUL;
      int b = buffer[2] * B_MUL;
      if (INVERT) {
        r = 255 - r;
        g = 255 - g;
        b = 255 - b;
      }

      SDL_SetRenderDrawColor(ren, r, g, b, 255);
      SDL_RenderDrawPoint(ren, ((w - neww) / 2) + x, y);
    }

    fclose(img);  

    SDL_RenderPresent(ren);
    if (!ROTATE) break;

    img = fopen(FILE_NAME, "r"); 
    fscanf(img, "%*s %*d %*d %*s");

    a++;
  }

  finish();
  return 0;
}

int init() {
  img = fopen(FILE_NAME, "r");

  if (RANDOMIZE_X || RANDOMIZE_Y) srand(time(0));

  fscanf(img, "%*s");
  fscanf(img, "%d", &w);
  fscanf(img, "%d", &h);
  fscanf(img, "%*s");

  pixelAmount = w * h;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(w, h, 0, &win, &ren);
}

void finish() {
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  fclose(img);
}
