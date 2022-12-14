#include "Map.h"

namespace Topo
{
	Map::Map()
	{
		const float screenWidth = static_cast<float>(GetScreenWidth());
		const float screenHeight = static_cast<float>(GetScreenHeight());

		unit = screenWidth / 160;
		jFMultiplier = 1;

		map.width = screenWidth;
		map.height = 90 * unit;
		map.x = 0;
		map.y = screenHeight / 2 - map.height / 2;

		tint = { 185, 235, 255,255 };

		player = new Character(unit, map);
		obs = new Obstacle(unit, map);
		bg = new Background(map);
		clouds = new Clouds(map);
	}

	Map::~Map()
	{
		delete player;
		delete obs;
	}

	Vector2 Map::getSize()
	{
		return { map.width, map.height };
	}

	float Map::getPosY()
	{
		return map.y;
	}

	float Map::getHeight()
	{
		return map.height;
	}

	float Map::getWidth()
	{
		return map.width;
	}

	void Map::update()
	{
		const float screenWidth = static_cast<float>(GetScreenWidth());
		const float screenHeight = static_cast<float>(GetScreenHeight());

		unit = screenWidth / 160;

		float playerYDif = player->getY();

		map.width = screenWidth;
		map.height = 90 * unit;
		map.x = 0;
		map.y = screenHeight / 2 - map.height / 2;

		input();

		checkCollisions();

		player->update(unit, map);
		obs->update(unit, map);
		bg->update(unit, map);
		clouds->update(unit, map);

		playerYDif = player->getY() - playerYDif;

		obs->updateParallax(unit, playerYDif);
		bg->updateParallax(unit, playerYDif);
		clouds->updateParallax(unit, playerYDif);

		if (obs->isBehindPlayer())
		{
			delete obs;
			obs = new Obstacle(unit, map);
		}
	}

	void Map::draw()
	{
		DrawRectangleRec(map, tint);
		clouds->draw();
		bg->draw();
		player->draw();
		obs->draw();
	}

	void Map::input()
	{
		if (IsKeyReleased(KEY_SPACE) || (IsKeyDown(KEY_SPACE) && jFMultiplier >= 2))
		{
			player->jump(jFMultiplier, unit);
			jFMultiplier = 1;
		}

		if (IsKeyDown(KEY_SPACE))
			jFMultiplier += GetFrameTime() * 2;
	}

	void Map::checkCollisions()
	{
		if (rectanglesCollide(player->getDest(), obs->getCol()))
			player->flicker();
	}
}