#include "image_loader.hpp"

ImageLoading::ImageLoading(SDL_Renderer* renderer) : m_renderer(renderer) {}

ImageLoading::~ImageLoading() {
    // Clean up if needed
}

SDL_Texture* ImageLoading::loadTexture(const std::string& path, int width, int height) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        SDL_Surface* resizedSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
        if (resizedSurface == nullptr) {
            std::cerr << "Unable to create surface for resizing! SDL Error: " << SDL_GetError() << std::endl;
        }
        else {
            SDL_BlitScaled(loadedSurface, nullptr, resizedSurface, nullptr);
            texture = SDL_CreateTextureFromSurface(m_renderer, resizedSurface);
            if (texture == nullptr) {
                std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
            }
            else {
                // Set blend mode to enable alpha blending
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            }
            SDL_FreeSurface(resizedSurface);
        }
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}