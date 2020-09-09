#include "utility.hpp"

core::GameState utility::create_default_state()
{
	using namespace core;

	auto default_state = core::GameState();

	default_state.entities[EntityNames::Orc] = core::Entity{ .health = 7, .name = "Orc" };
	default_state.entities[EntityNames::Dragon] = core::Entity{ .health = 20, .name = "Dragon" };
	default_state.entities[EntityNames::Player] = core::Entity{ .health = 40, .name = "Hero" };
	default_state.current_events.clear();
	default_state.future_events.clear();
	default_state.shape = GameShape::Running;

	return default_state;
}