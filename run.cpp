#include <memory>

#include "sdladapters.h"
#include "scene.h"
#include "sprite.h"

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
    scene.add(runner, {5, 220, 120, 80});

    pump.run([&]() { scene.render(); }, 16, [](auto ev) {});
    return 0;
}