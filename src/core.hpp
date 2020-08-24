#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <array>
#include <variant>

namespace core
{
	/*************************************************************/
	enum class GameShape
	{
		Running,
		Menu,
		Won,
		Lost,
		Restart
	};

	enum class EntityNames
	{
		Orc = 0,
		Dragon = 1,
		Player = 2
	};

	struct Entity
	{
		int health;
		std::string name;
	};

	struct DealDamageEvent
	{
		EntityNames source;
		EntityNames target;
		int damage;
	};

	struct GameShapeChangeEvent
	{
		GameShape new_shape;
	};

	using Event = std::variant<DealDamageEvent, GameShapeChangeEvent>;
	using EntityContainer = std::array<Entity, 3>;

	struct GameState
	{
		GameShape shape;
		EntityContainer entities;
		std::string current_input;
		std::vector<Event> current_events;
	};
}