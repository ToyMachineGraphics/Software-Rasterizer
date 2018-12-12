#ifndef SDL_WRAPPER_H_INCLUDED
#define SDL_WRAPPER_H_INCLUDED

#include "SDL/SDL.h"
#include <functional>
#include <cstdint>

using std::function;

// ==== Window ==== //
extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool CreateWindow(int width, int height);
void Predraw();
void Postdraw();
void SetColor(float r, float g, float b, float a);
void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void DrawPixel(int x, int y);
void Clear();




// ==== Control ==== //
extern function<void(int width, int height)> onCreateWindow;
extern function<void(void)> onQuit;
extern function<void(void)> onLeftKeyDown;
extern function<void(void)> onRightKeyDown;
extern function<void(void)> onUpKeyDown;
extern function<void(void)> onDownKeyDown;
extern function<void(void)> onCKeyDown;
extern function<void(void)> onDKeyDown;
extern function<void(void)> onSpaceKeyDown;

void HandleEvents();

#endif // SDL_WRAPPER_H_INCLUDED
