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

std::vector<sRoom> g_rooms = {
    {7, 7,
    "# # # # # # # "
    "# t^. # . t^# "
    "# . . # . . # "
    "+ . . t^. . + "
    "# t<. p . . # "
    "# . . . . t,# "
    "# # # + # # # "},
    {7, 7,
    "# # # + # # # "
    "# t^. . . t^# "
    "# . . . . . # "
    "+ . . p . . + "
    "# . . . . . # "
    "# t,. . . t,# "
    "# # # + # # # "},
    {7, 7,
    "  # # + # #   "
    "# # t<. . # # "
    "# . . . . t^# "
    "+ . . p . . + "
    "# t,. . . . # "
    "# # . . t># # "
    "  # # + # #   "},
    {7, 5,
    "# # # + # # # "
    "# . . t^. . # "
    "+ . . p . . + "
    "# t,. . . t,# "
    "# # # + # # # "},
    {9, 5,
    "# # + # # # + # # "
    "# . . . t^. . . # "
    "+ . . . p . . . + "
    "# t,. . . . . t># "
    "# # # # + # # # # "},
    {9, 5,
    "# # # # # # # # # "
    "# . t^. . . t^. # "
    "+ . . . p . . . + "
    "# t,. . . . . t># "
    "# # # # # # # # # "},
    {13, 5,
    "# # # # + # # # # # # # # "
    "# . t^. . . t^. . . t^. # "
    "+ . . . . . p . . . . . + "
    "# t,. . . t,. . . . . t># "
    "# # # # + # # # # # # # # "},
    {5, 5,
    "# # + # # "
    "# . . t># "
    "+ . p . + "
    "# . . . # "
    "# # + # # "},
    {4, 4,
    "# + # # "
    "+ . t># "
    "# p . + "
    "# # + # "},
    {7, 7,
    "    # # + # # "
    "    # . . t^# "
    "# # # . . . # "
    "# t^. p . . + "
    "+ . . . . . # "
    "# . . . . t,# "
    "# # # + # # # "},
    {9, 9,
    "# # + # + # + # # "
    "# . . t^. . . . # "
    "+ . . . . . . . + "
    "# t<. # . # . t># "
    "+ . . . p . . . + "
    "# . . # . # . t># "
    "+ . . . . . . . + "
    "# . . t,. . . . # "
    "# # + # + # + # # "},
    {9, 9,
    "# # # # + # + # # "
    "# . . t^. . . . # "
    "# . . . . . . . + "
    "# . . #<t . . t># "
    "+ . t># p . . . + "
    "# . . # . . . t># "
    "# . . . . . . . + "
    "# . . t,. . . . # "
    "# # # # + # + # # "},
    {9, 9,
    "      # # + + # # "
    "      # . . . . # "
    "      # t<. . . + "
    "# # # # . . . t># "
    "# . t^. p . . . + "
    "+ . . . . . . . # "
    "+ . . . . . . t>+ "
    "# . . t,. . . . # "
    "# # + # + # + # # "},
    {10, 10,
    "        # # + + # # "
    "        # . . . . # "
    "        + . . . . # "
    "        # t<. . . + "
    "# # + # # . . . t># "
    "# . . t^. p . . . # "
    "+ . . . . . . . . # "
    "+ . . . . . . . t>+ "
    "# . . t,. . . . . # "
    "# # + # + # + # # # "},
    {11, 8,
    "      # # + # #       "
    "      # t<. . #       "
    "      # . . . #       "
    "# # # # . . . # # # # "
    "# . . t^. . . t^. . # "
    "+ . . . . p . . . . + "
    "# t,. . . . . . . t,# "
    "# # + # # + # # + # # "},
    {15, 15,
    "        # # + # + # #         "
    "        # . . t^. . #         "
    "        # . . . . . #         "
    "      # . . . . . . . #       "
    "# # # . . . . . . . . . # # # "
    "# t<. . . . . t,. . . . . t># "
    "# . . . . . # # # . . . . . # "
    "+ . . . . t>#   # t<. . . . + "
    "# . . . . . # # # . . . . . # "
    "# t<. . . . . t^. . . . . t># "
    "# # # . . p . . . . . . # # # "
    "      # . . . . . . . #       "
    "        # . . . . . #         "
    "        # . . t,. . #         "
    "        # # + # + # #         "},
};

onut::Entity* LevelGenerator::createTorch(const glm::vec2& position)
{
    auto pEntity = onut::EntityFactory::createPointLight({position.x, position.y, 0.85f}, 3.f, {2.f, 1.5f, 1.f, 1.f});
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
            return -90.f;
        case ',':
            return 180.f;
        case '<':
            return 90.f;
        case ' ':
            return static_cast<float>(onut::randi(3) * 90);
        default:
            return 0.f;
    }
}

char rotateModifier(char modifier, int rotation)
{
    switch (rotation)
    {
        case 1:
            switch (modifier)
            {
                case '^':
                    return '<';
                case '>':
                    return '^';
                case ',':
                    return '>';
                case '<':
                    return ',';
            }
            break;
        case 2:
            switch (modifier)
            {
                case '^':
                    return ',';
                case '>':
                    return '<';
                case ',':
                    return '^';
                case '<':
                    return '>';
            }
            break;
        case 3:
            switch (modifier)
            {
                case '^':
                    return '>';
                case '>':
                    return ',';
                case ',':
                    return '<';
                case '<':
                    return '^';
            }
            break;
    }

    return modifier;
}

bool isEmpty(char tile)
{
    return tile == ' ' || tile == '\0';
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

bool isDoor(char tile)
{
    return tile == '+';
}

onut::Entity* LevelGenerator::createRoom(int startX, int startY, const sRoom& room)
{
    auto pRoomEntity = new onut::Entity();
    pRoomEntity->setPosition({static_cast<float>(startX), static_cast<float>(startY), 0.f});

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
                pEntity = onut::EntityFactory::createMesh({static_cast<float>(x), static_cast<float>(y), 0.f},
                                                          "assets/meshes/wall1.mesh", "assets/materials/wall1.material");
            }
            else if (isFloor(tileId))
            {
                if (onut::randb(.04f))
                {
                    pEntity = onut::EntityFactory::createMesh({static_cast<float>(x), static_cast<float>(y), 0.f},
                                                              "assets/meshes/floor2.mesh", "assets/materials/floor2.material");
                }
                else
                {
                    pEntity = onut::EntityFactory::createMesh({static_cast<float>(x), static_cast<float>(y), 0.f},
                                                              "assets/meshes/floor.mesh", "assets/materials/floor.material");
                }
            }

            // Addons
            if (pEntity)
            {
                if (isTorch(tileId) && onut::randb(.85f))
                {
                    pEntity->add(createTorch({0.f, 0.75f}));
                }
                pEntity->setRotation({0, 0, getAngle(modifier)});
            }

            pRoomEntity->add(pEntity);
        }
    }

    return pRoomEntity;
}

void blitRoom(sRoom& dst, const sRoom& src, int mapX, int mapY, int rotation)
{
    for (int srcY = 0; srcY < src.h; ++srcY)
    {
        for (int srcX = 0; srcX < src.w; ++srcX)
        {
            auto tileId = src.tiles[srcY * src.w * 2 + srcX * 2 + 0];
            auto modifier = src.tiles[srcY * src.w * 2 + srcX * 2 + 1];

            char dstTileId;
            char dstModifier;
            switch (rotation)
            {
                case 0:
                    dstTileId = dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 1];
                    break;
                case 1:
                    dstTileId = dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 1];
                    break;
                case 2:
                    dstTileId = dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 1];
                    break;
                case 3:
                    dstTileId = dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 1];
                    break;
            }

            modifier = rotateModifier(modifier, rotation);

            if (tileId == '+' && dstTileId == '+' &&
                onut::randb(.85f))
            {
                tileId = '.';
            }
            else if (isWall(dstTileId))
            {
                tileId = '#';
            }

            switch (rotation)
            {
                case 0:
                    dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 0] = tileId;
                    dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 1] = modifier;
                    break;
                case 1:
                    dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 0] = tileId;
                    dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 1] = modifier;
                    break;
                case 2:
                    dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 0] = tileId;
                    dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 1] = modifier;
                    break;
                case 3:
                    dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 0] = tileId;
                    dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 1] = modifier;
                    break;
            }
        }
    }
}

bool roomFits(sRoom& dst, const sRoom& src, int mapX, int mapY, int rotation)
{
    if (mapX < 0 || mapY < 0) return false;
    switch (rotation)
    {
        case 0:
        case 2:
            if (mapX + src.w > dst.w) return false;
            if (mapY + src.h > dst.h) return false;
            break;
        case 1:
        case 3:
            if (mapX + src.h > dst.w) return false;
            if (mapY + src.w > dst.h) return false;
            break;
    }

    for (int srcY = 0; srcY < src.h; ++srcY)
    {
        for (int srcX = 0; srcX < src.w; ++srcX)
        {
            const auto& tileId = src.tiles[srcY * src.w * 2 + srcX * 2 + 0];
            auto modifier = src.tiles[srcY * src.w * 2 + srcX * 2 + 1];

            char dstTileId;
            char dstModifier;

            modifier = rotateModifier(modifier, rotation);

            switch (rotation)
            {
                case 0:
                    dstTileId = dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + srcY)* dst.w * 2 + (mapX + srcX) * 2 + 1];
                    break;
                case 1:
                    dstTileId = dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + src.h - 1 - srcX)* dst.w * 2 + (mapX + srcY) * 2 + 1];
                    break;
                case 2:
                    dstTileId = dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + src.h - 1 - srcY)* dst.w * 2 + (mapX + src.w - 1 - srcX) * 2 + 1];
                    break;
                case 3:
                    dstTileId = dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 0];
                    dstModifier = dst.tiles[(mapY + srcX)* dst.w * 2 + (mapX + src.w - 1 - srcY) * 2 + 1];
                    break;
            }

            if (!((
                (isWall(tileId) || isEmpty(tileId)) &&
                (isWall(dstTileId) || isEmpty(dstTileId))
                ) || 
                isEmpty(dstTileId)))
            {
                return false;
            }
        }
    }
    return true;
}

void findAllDoors(std::vector<int>& outDoors, const sRoom& room)
{
    outDoors.clear();
    auto len = room.w * room.h;
    for (auto i = 0; i < len; ++i)
    {
        if (isDoor(room.tiles[i * 2]))
        {
            outDoors.push_back(i);
        }
    }
}

void LevelGenerator::onCreate()
{
    // Make sure we have a parent to attach it to
    auto pParent = getParent();
    if (!pParent) return;

    // Static map data
    struct sMap
    {
        sRoom room;
        sMap()
        {
            room.w = 128;
            room.h = 128;
            room.tiles = new char[room.w * room.h * 2];
        }
    };
    static sMap map;
    memset(map.room.tiles, 0, sizeof(char) * map.room.w * map.room.h * 2);

    // Pick/Place starting room
    auto& startRoom = onut::randv(g_rooms);
    blitRoom(map.room, startRoom, map.room.w / 2 - startRoom.w / 2, map.room.h / 2 - startRoom.h / 2, onut::randi(0, 3));

    // How many rooms we are going to have
    auto roomCount = onut::randi(5, 8) - 1;
    static std::vector<int> doors;
    static std::vector<int> roomDoors;
    for (auto i = 0; i < roomCount; ++i)
    {
        bool bGotIt = false;
        for (auto tries1 = 0; tries1 < 10 && !bGotIt; ++tries1)
        {
            // Find a door emplacement
            findAllDoors(doors, map.room);
            if (doors.empty()) break; // That's it, we're done

            // Pick a random door
            auto door = onut::randv(doors);
            int doorX = door % map.room.w;
            int doorY = door / map.room.w;

            // Find a room that will fit there, and give it a lot of tries
            for (auto tries = 0; tries < 100 && !bGotIt; ++tries)
            {
                auto& room = onut::randv(g_rooms);
                auto rotation = onut::randi(0, 3);
                findAllDoors(roomDoors, room);
                onut::shuffle(roomDoors);
                for (auto roomDoor : roomDoors)
                {
                    int roomDoorX = roomDoor % room.w;
                    int roomDoorY = roomDoor / room.w;
                    int roomOffsetX = 0;
                    int roomOffsetY = 0;

                    switch (rotation)
                    {
                        case 0:
                            roomOffsetX = -roomDoorX;
                            roomOffsetY = -roomDoorY;
                            break;
                        case 1:
                            roomOffsetX = -roomDoorY;
                            roomOffsetY = -(room.h - roomDoorX - 1);
                            break;
                        case 2:
                            roomOffsetX = -(room.w - roomDoorX - 1);
                            roomOffsetY = -(room.h - roomDoorY - 1);
                            break;
                        case 3:
                            roomOffsetX = -(room.w - roomDoorY - 1);
                            roomOffsetY = -roomDoorX;
                            break;
                    }

                    // Check if it fits
                    if (roomFits(map.room, room, doorX + roomOffsetX, doorY + roomOffsetY, rotation))
                    {
                        blitRoom(map.room, room, doorX + roomOffsetX, doorY + roomOffsetY, rotation);
                        map.room.tiles[door * 2] = '.';
                        bGotIt = true;
                        break;
                    }
                }
            }
        }
    }

    pParent->add(createRoom(0, 0, map.room));
}
