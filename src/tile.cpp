// Example program:
// Using SDL2 to create an application window and show bmp image

#include "SDL.h"
#include "cleanup.hpp"
#include <iostream>


const int SCREEN_WIDTH  = 320;
const int SCREEN_HEIGHT = 320;


/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}


/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Make sure converting went ok too
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}


/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}


int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *background = loadTexture("../fixtures/background.bmp", renderer);
    SDL_Texture *image = loadTexture("../fixtures/smile.bmp", renderer);
    if (background == nullptr || image == nullptr) {
        cleanup(background, image, renderer, window);
        SDL_Quit();
        return 1;
    }

    SDL_Event ev;
    // Main event loop. push x button to end the loop
    while (true){
        while(SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                cleanup(background, image, renderer, window);
                SDL_Quit();
                return 0;
            }
        }

        SDL_RenderClear(renderer);

        int bW, bH;
        SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
        renderTexture(background, renderer, 0, 0);
        renderTexture(background, renderer, bW, 0);
        renderTexture(background, renderer, 0, bH);
        renderTexture(background, renderer, bW, bH);


        int iW, iH;
        SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
        int x = SCREEN_WIDTH / 2 - iW / 2;
        int y = SCREEN_HEIGHT / 2 - iH / 2;
        renderTexture(image, renderer, x, y);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
