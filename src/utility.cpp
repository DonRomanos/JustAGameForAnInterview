#include "utility.hpp"

core::GameState utility::create_default_state()
{
	using namespace core;

	auto game = core::GameState();

	game.entities[EntityNames::Orc] = core::Entity{ .health = 7, .name = "Orc" };
	game.entities[EntityNames::Dragon] = core::Entity{ .health = 20, .name = "Dragon" };
	game.entities[EntityNames::Player] = core::Entity{ .health = 40, .name = "Hero" };
	game.current_events.clear();
	game.future_events.clear();
	game.shape = GameShape::Restart;
	game.future_shape = GameShape::Running;

	return game;
}