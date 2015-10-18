#include "LevelGenerator.h"
#include "Torch.h"

#include <ContentManager.h>
#include <Entity.h>
#include <EntityFactory.h>
#include <Game.h>
#include <Material.h>
#include <Mesh.h>
#include <MeshRenderer.h>
#include <PointLight.h>
#include <Random.h>

struct sRoom
{
    int32_t w;
    int32_t h;
    char tiles[16 * 16 * 2];
};

std::vector<sRoom> g_rooms = {
    {7, 7,
    "# # # + # # # "
    "# t^. . . t^# "
    "# . . . . . # "
    "+ . . p . . + "
    "# . . . . . # "
    "# t,. . . t,# "
    "# # # + # # # "}
};

onut::Entity* LevelGenerator::createTorch(const glm::vec2& position)
{
    auto pEntity = onut::EntityFactory::createPointLight({position.x, position.y, 1.7f}, 5.f, {2.f, 1.5f, 1.f, 1.f});
    pEntity->addComponent<Torch>();
    return pEntity;
}

float getAngle(char modifier)
{
    switch (modifier)
    {
        case '^':
            return 0.f;
        case '>':
            return 90.f;
        case ',':
            return 180.f;
        case '<':
            return 270.f;
        case ' ':
            return static_cast<float>(onut::randi(3) * 90);
        default:
            return 0.f;
    }
}

bool isWall(char tile)
{
    return tile == '#' || tile == '+';
}

bool isTorch(char tile)
{
    return tile == 't';
}

bool isPlayer(char tile)
{
    return tile == 'p';
}

bool isFloor(char tile)
{
    return tile == '.' || isTorch(tile) || isPlayer(tile);
}

void LevelGenerator::onCreate()
{
    const auto& room = onut::randv(g_rooms);

    auto pParent = getParent();
    if (pParent)
    {
        // Create floor. 5x5
        for (int y = 0; y < room.h; ++y)
        {
            for (int x = 0; x < room.w; ++x)
            {
                const auto& tileId = room.tiles[(room.h - y - 1) * room.w * 2 + x * 2];
                const auto& modifier = room.tiles[(room.h - y - 1) * room.w * 2 + x * 2 + 1];

                onut::Entity* pEntity = nullptr;

                // Solid
                if (isWall(tileId))
                {
                }
                else if (isFloor(tileId))
                {
                    pEntity = onut::EntityFactory::createMesh({static_cast<float>(x)* 2.f, static_cast<float>(y)* 2.f, 0.f},
                                                              "assets/meshes/floor.mesh", "assets/materials/floor.material");
                }

                // Addons
                if (pEntity)
                {
                    if (isTorch(tileId))
                    {
                        pEntity->add(createTorch({0.f, 1.f}));
                    }

                    pEntity->setRotation({0, 0, getAngle(modifier)});
                    pParent->add(pEntity);
                }
            }
        }
    }
}
