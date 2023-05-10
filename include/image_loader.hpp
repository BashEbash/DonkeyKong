#ifndef IMAGE_LOADING_HPP
#define IMAGE_LOADING_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

class ImageLoading {
public:
    ImageLoading(SDL_Renderer* renderer);
    ~ImageLoading();

    SDL_Texture* loadTexture(const std::string& path, int width, int height);

private:
    SDL_Renderer* m_renderer;
};

#endif // IMAGE_LOADING_HPP