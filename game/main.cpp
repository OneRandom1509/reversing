#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;

struct SnakeSegment {
  int x, y;
};

std::vector<SnakeSegment> snake = {{400, 300}};
int foodX, foodY;
int directionX = GRID_SIZE, directionY = 0;
bool running = true;
long long int score = 00000;

void placeFood() {
  foodX = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
  foodY = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
}

void handleInput(SDL_Event &e) {
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      running = false;
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = false;
        break;
      case SDLK_UP:
        if (directionY == 0) {
          directionX = 0;
          directionY = -GRID_SIZE;
        }
        break;
      case SDLK_DOWN:
        if (directionY == 0) {
          directionX = 0;
          directionY = GRID_SIZE;
        }
        break;
      case SDLK_LEFT:
        if (directionX == 0) {
          directionX = -GRID_SIZE;
          directionY = 0;
        }
        break;
      case SDLK_RIGHT:
        if (directionX == 0) {
          directionX = GRID_SIZE;
          directionY = 0;
        }
        break;
      }
    }
  }
}

void updateGame() {
  SnakeSegment newHead = {snake[0].x + directionX, snake[0].y + directionY};
  if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 ||
      newHead.y >= SCREEN_HEIGHT) {
    running = false;
    return;
  }
  for (auto &segment : snake) {
    if (segment.x == newHead.x && segment.y == newHead.y) {
      running = false;
      return;
    }
  }
  snake.insert(snake.begin(), newHead);
  if (newHead.x == foodX && newHead.y == foodY) {
    score += 10;
    placeFood();
  } else {
    snake.pop_back();
  }
}

void renderGame(SDL_Renderer *renderer, TTF_Font *font) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect foodRect = {foodX, foodY, GRID_SIZE, GRID_SIZE};
  SDL_RenderFillRect(renderer, &foodRect);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (auto &segment : snake) {
    SDL_Rect snakeRect = {segment.x, segment.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &snakeRect);
  }

  SDL_Color white = {255, 255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(
      font, ("Score: " + std::to_string(score)).c_str(), white);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect destRect = {10, 10, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &destRect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  SDL_RenderPresent(renderer);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  SDL_Window *window = SDL_CreateWindow(
      "Vulnerable Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  TTF_Font *font = TTF_OpenFont("SpaceMonoNerdFont-Regular.ttf", 24);

  srand(time(0));
  placeFood();
  SDL_Event e;

  while (running) {
    handleInput(e);
    updateGame();
    renderGame(renderer, font);
    SDL_Delay(100);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
