#include "context.h"
#include "lib/SDL_wrapper.h"
#include <memory>
#include <cstdio>
#include <iostream>
#include <algorithm>

using std::shared_ptr;
using std::cerr;
using std::endl;
using std::max;
using std::min;
using std::round;

using SoftwareRenderer::Context;

SDL_Window* window;
SDL_Renderer* renderer;
function<void(int width, int height)> onCreateWindow;

bool CreateWindow(int width, int height)
{
    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(window == nullptr) {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);
    if (renderer == nullptr) {
        return false;
    }

    shared_ptr<SDL_RendererInfo> rendererInfo(new SDL_RendererInfo());
    if (SDL_GetRendererInfo(renderer, rendererInfo.get()) < 0) {
        cerr << "Couldn't get SDL 2D rendering driver information: " << SDL_GetError() << endl;
        return false;
    }
    printf("Rendering driver in use: %s\n", rendererInfo->name);
    printf("    SDL_RENDERER_SOFTWARE     [%c]\n", (rendererInfo->flags & SDL_RENDERER_SOFTWARE) ? 'X' : ' ');
    printf("    SDL_RENDERER_ACCELERATED  [%c]\n", (rendererInfo->flags & SDL_RENDERER_ACCELERATED) ? 'X' : ' ');
    printf("    SDL_RENDERER_PRESENTVSYNC [%c]\n", (rendererInfo->flags & SDL_RENDERER_PRESENTVSYNC) ? 'X' : ' ');

	SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_NONE);

	onCreateWindow(width, height);

    return true;
}

void Predraw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void Postdraw()
{
    SDL_RenderPresent(renderer);
}

void SetColor(float r, float g, float b, float a)
{
	uint8_t u8r = (uint8_t)round(max(0.0f, min(r, 1.0f)) * 255);
	uint8_t u8g = (uint8_t)round(max(0.0f, min(g, 1.0f)) * 255);
	uint8_t u8b = (uint8_t)round(max(0.0f, min(b, 1.0f)) * 255);
	uint8_t u8a = (uint8_t)round(max(0.0f, min(a, 1.0f)) * 255);
    SDL_SetRenderDrawColor(renderer, u8r, u8g, u8b, u8a);
}

void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void DrawPixel(int x, int y)
{
	SDL_RenderDrawPoint(renderer, x, context.Dimension().second - y - 1);
}

function<void(void)> onQuit;
function<void(void)> onLeftKeyDown;
function<void(void)> onRightKeyDown;
function<void(void)> onUpKeyDown;
function<void(void)> onDownKeyDown;
function<void(void)> onCKeyDown;
function<void(void)> onDKeyDown;
function<void(void)> onSpaceKeyDown;

void HandleEvents()
{
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            onQuit();
            break;
        case SDL_KEYDOWN:
        {
            switch( event.key.keysym.sym ) {
            case SDLK_LEFT:
                onLeftKeyDown();
                break;
            case SDLK_RIGHT:
                onRightKeyDown();
                break;
			case SDLK_UP:
				onUpKeyDown();
				break;
			case SDLK_DOWN:
				onDownKeyDown();
				break;
			case SDLK_d:
				onDKeyDown();
				break;
			case SDLK_c:
				onCKeyDown();
				break;
            case SDLK_SPACE:
                onSpaceKeyDown();
                break;
            default:
                break;
            }
        }
        default:
            break;
        }
    }
}

void Clear()
{
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
