#include "game/ReferencePaths.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string_view>

namespace {

bool HasArg(int argc, char **argv, std::string_view expected)
{
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(argv[i]) == expected) {
            return true;
        }
    }
    return false;
}

void PrintReferenceStatus()
{
    const auto projectRoot = th07::FindProjectRoot();
    const auto originalRoot = th07::OriginalReferenceRoot(projectRoot);

    std::cout << "Project root: " << projectRoot << '\n';
    std::cout << "Original reference root: " << originalRoot << '\n';

    for (const th07::ReferenceFile &file : th07::RequiredOriginalFiles()) {
        const auto path = originalRoot / file.relativePath;
        std::cout << "  " << file.name << ": " << (std::filesystem::exists(path) ? "ok" : "missing") << " ("
                  << path << ")\n";
    }
}

} // namespace

int main(int argc, char **argv)
{
    PrintReferenceStatus();

    if (HasArg(argc, argv, "--check-resources")) {
        return th07::HasRequiredOriginalFiles() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if (HasArg(argc, argv, "--headless-smoke")) {
        std::cout << "Headless smoke passed; SDL/Vulkan window creation was intentionally skipped.\n";
        return th07::HasRequiredOriginalFiles() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    const bool smoke = HasArg(argc, argv, "--smoke");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD) == false) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        if (smoke) {
            std::cerr << "Smoke mode is running without an available video device; treating this as a headless skip.\n";
            return th07::HasRequiredOriginalFiles() ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (SDL_Vulkan_LoadLibrary(nullptr) == false) {
        std::cerr << "SDL_Vulkan_LoadLibrary failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Uint32 extensionCount = 0;
    const char *const *extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
    if (extensions == nullptr || extensionCount == 0) {
        std::cerr << "SDL_Vulkan_GetInstanceExtensions failed: " << SDL_GetError() << '\n';
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("th07-vulkan", 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    std::cout << "SDL Vulkan extension count: " << extensionCount << '\n';

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        if (smoke) {
            running = false;
        } else {
            SDL_Delay(16);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Vulkan_UnloadLibrary();
    SDL_Quit();
    return EXIT_SUCCESS;
}
