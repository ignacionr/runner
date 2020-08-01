#include <memory>

#include "sdladapters.h"
#include "scene.h"
#include "sprite.h"
#include "transportable.h"

int main()
{
    sdl::Window window("Run!", 700, 300);
    sdl::EventPump pump;
    auto renderer = std::make_shared<sdl::Renderer>(window);
    sdls::Scene scene(renderer);
    scene.background("IMG_6110.jpg", {0, 150, 500, 200}, [](auto &rc) {
        ++rc.x %= (2338 - rc.w);
    });
    Sprite runner(renderer, "tile.png", 4, 7);
    Transportable tr(runner);

    auto &main_character = scene.add(tr, {-40, 220, 120, 80});
    tr.towards({505, 220, 120, 80});
    scene.interact([&](int cx, int _) {
        SDL_Rect destination{main_character->area};
        destination.x += cx * 20;
        tr.towards(destination);
    });

    pump.run(
        std::bind(&sdls::Scene::render, &scene),
        16,
        std::bind(&sdls::Scene::handle_event, &scene, std::placeholders::_1));
    return 0;
}