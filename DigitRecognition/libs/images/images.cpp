
#include "images.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

#if HAS_SDL_IMAGE
#include <SDL_image.h>
#include <SDL.h>

// SDL initialization helper
class SDL_Manager {
public:
    SDL_Manager() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            initialized = false;
            return;
        }
        
        int img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags) & img_flags)) {
            std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
            SDL_Quit();
            initialized = false;
            return;
        }
        
        initialized = true;
    }
    
    ~SDL_Manager() {
        if (initialized) {
            IMG_Quit();
            SDL_Quit();
        }
    }
    
    bool is_initialized() const { return initialized; }
    
private:
    bool initialized = false;
};

// Global SDL manager (will be initialized once)
static SDL_Manager& get_sdl_manager() {
    static SDL_Manager manager;
    return manager;
}
#endif

std::vector<double> ANN::load_image(const std::string& filename) {
    // std::cout << "Loading image from: " << filename << std::endl;

    // Check if file exists
    if (!std::filesystem::exists(filename)) {
        std::cout << "File does not exist: " << filename << std::endl;
        return {};  // Return empty vector for non-existent files
    }

    // Check if it's a valid file extension
    std::string extension = std::filesystem::path(filename).extension().string();
    if (extension != ".png" && extension != ".bmp" && extension != ".jpg" && extension != ".jpeg") {
        std::cout << "Unsupported file format: " << extension << std::endl;
        return {};  // Return empty vector for unsupported formats
    }

    // Real SDL2_image implementation
    auto& sdl_manager = get_sdl_manager();
    if (!sdl_manager.is_initialized()) {
        std::cerr << "SDL not initialized, err, help" << std::endl;
        // Fall through to simulation code below
        return {};
    } 

    // std::cout << "Using SDL2_image for real image loading..." << std::endl;
    
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
        return {};
    }
    
    // std::cout << "Loaded surface: " << surface->w << "x" << surface->h 
    //             << " format: " << SDL_GetPixelFormatName(surface->format->format) << std::endl;
    
    std::vector<double> pixels;
    pixels.reserve(surface->w * surface->h);
    
    // Lock surface for pixel access
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }
    
    // Convert pixels to grayscale double values (0-255)
    Uint8* pixel_data = static_cast<Uint8*>(surface->pixels);
    int bytes_per_pixel = surface->format->BytesPerPixel;
    
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint8* pixel = &pixel_data[(y * surface->pitch) + (x * bytes_per_pixel)];
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(*reinterpret_cast<Uint32*>(pixel), surface->format, &r, &g, &b, &a);
            
            // Convert to grayscale using luminance formula
            double gray = 0.299 * r + 0.587 * g + 0.114 * b;
            pixels.push_back(gray);
        }
    }
    
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
    
    SDL_FreeSurface(surface);
    
    // std::cout << "Successfully loaded " << pixels.size() << " pixels with SDL2_image" << std::endl;
    return pixels;
}

