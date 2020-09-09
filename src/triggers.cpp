#include "triggers.hpp"
#include "utility.hpp"
#include "input_provider.hpp"

void triggers::EventFactory::execute(core::GameState& state, const std::chrono::milliseconds elapsed_time)
{
	for (auto& trigger_info : triggers)
	{
		if (!trigger_info.execution_condition || trigger_info.execution_condition(state)) // Trigger has no condition or condition is true.
		{
			std::visit(utility::overloaded{
				[&state, &trigger_info](InputTrigger trigger) 
				{ 
					if (trigger.executes(state)) 
						state.future_events.push_back(trigger_info.generated_event); 
				},
				[elapsed_time, &state, &trigger_info](TimedTrigger& trigger) 
				{ 
					// TimedTriggers only execute when the game is actually running.
					if (!utility::game_is_running(state))
					{
						return;
					}
					if (trigger.executes(elapsed_time)) 
						state.future_events.push_back(trigger_info.generated_event);
				}
			}, trigger_info.trigger);
		}
	}
}

void triggers::EventFactory::add_trigger(TriggerInfo&& to_add)
{
	triggers.emplace_back(std::move(to_add));
}

triggers::TimedTrigger::TimedTrigger(const std::chrono::milliseconds cycle)
	: cycle_duration(cycle)
	, counter(0)
{
}

triggers::TimedTrigger::TimedTrigger(const std::chrono::milliseconds cycle, const std::chrono::milliseconds counter)
	: cycle_duration(cycle)
	, counter(counter)
{
}

bool triggers::TimedTrigger::executes(const std::chrono::milliseconds elapsed_time)
{
	counter += elapsed_time;
	if (counter >= cycle_duration)
	{
		counter = counter - cycle_duration;
		return true;
	}
	return false;
}

triggers::InputTrigger::InputTrigger(std::string_view phrase, input::ProviderI& in)
	: trigger_phrase(phrase) 
	, source(in)
{

}

bool triggers::InputTrigger::executes(core::GameState& state)
{
	if (state.current_input.starts_with(trigger_phrase + "\n"))
	{
		return true;
	}
	return false;
}
