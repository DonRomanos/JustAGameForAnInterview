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
	constexpr std::size_t NumberOfEntities = 3;

	struct Entity
	{
		int health;
		std::string name;
	};

	// Necessary so we do not have to static cast everywhere.
	template<std::size_t Size>
	class EntityArray : public std::array<Entity, Size>
	{
		using parent = std::array<Entity, Size>;
	public:
		[[nodiscard]] constexpr Entity& operator[](const EntityNames name) noexcept { return parent::operator[](static_cast<size_t>(name)); }
		[[nodiscard]] constexpr const Entity& operator[](const EntityNames name) const noexcept { return parent::operator[](static_cast<size_t>(name)); }
	};

	struct DealDamageEvent
	{
		EntityNames source;
		EntityNames target;
		int damage;
	};

	struct KillEvent
	{
		EntityNames killer;
		EntityNames victim;
	};

	using Event = std::variant<DealDamageEvent, KillEvent>;
	using EntityContainer = EntityArray<NumberOfEntities>;

	struct GameState
	{
		EntityContainer entities;
		std::string current_input; // We need to keep track of the state in order to solve the scrambled input problem!  

		// double buffering just like rendering engines do ;)
		std::vector<Event> current_events;
		std::vector<Event> future_events;

		GameShape shape;
		GameShape future_shape;
	};
}