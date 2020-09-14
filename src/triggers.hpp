#pragma once

#include "core.hpp"
#include "system.hpp"

#include <variant>
#include <vector>
#include <chrono>
#include <optional>
#include <string_view>
#include <concepts>
#include <type_traits>
#include <utility>
#include <functional>

namespace input
{
	class ProviderI;
}

namespace triggers
{
	class InputTrigger
	{
	public:
		explicit InputTrigger(std::string_view phrase, input::ProviderI& source);

		bool executes(core::GameState& state);
	private:
		input::ProviderI& source;
		std::string trigger_phrase;
	};

	class TimedTrigger
	{
	public:
		explicit TimedTrigger(const std::chrono::milliseconds cycle);
		TimedTrigger(const std::chrono::milliseconds cycle, const std::chrono::milliseconds counter);

		bool executes(const std::chrono::milliseconds elapsed_time);
		void reset_counter();
	private:
		std::chrono::milliseconds cycle_duration;
		std::chrono::milliseconds counter;
	};

	/*************************************************************/
	using Trigger = std::variant<InputTrigger, TimedTrigger>;
	using Condition = std::function<bool(const core::GameState&)>;

	//template<typename T>
	//concept TriggerCondition = std::is_invocable_r<bool, const core::GameState&>::value;

	// Somehow I could not make it work directly with the above maybe I dont yet understand enough about concepts.
	template<typename T>
	concept TriggerCondition = requires(T&& f, const core::GameState& arg)
	{
		{std::invoke(std::forward<T>(f), arg)} -> std::convertible_to<bool>;
	};

	template<TriggerCondition ...Args>
	auto all_of(Args ...args)
	{
		return[... args = std::forward<Args>(args)](const core::GameState& state)
		{
			return (... && args(state));
		};
	}

	template<TriggerCondition ...Args>
	auto any_of(Args ...args)
	{
		return[... args = std::forward<Args>(args)](const core::GameState& state)
		{
			return (... || args(state));
		};
	}

	/************************************************************/
	struct TriggerInfo
	{
		Trigger trigger;
		Condition execution_condition;
		core::Event generated_event;
	};

	template<typename T>
	class TriggerBuilder
	{
	public:
		template<typename ...Types>
		TriggerBuilder(Types&& ... args);

		[[nodiscard]] TriggerBuilder& aslong_as(const Condition& requirement) 
		{ 
			to_build.execution_condition = requirement; 
			return *this;
		}
		
		[[nodiscard]] TriggerInfo generate_event(const core::Event& event) 
		{ 
			to_build.generated_event = event;
			return to_build;
		}

	private:
		TriggerInfo to_build;
	};

	using Every = TriggerBuilder<TimedTrigger>;
	using OnPhrase = TriggerBuilder<InputTrigger>;

	/*****************************************************************/
	class EventFactory : public core::TimedSystem
	{
	public:
		virtual void execute(core::GameState& state, const std::chrono::milliseconds elapsed_time) override;
		
		void add_trigger(TriggerInfo&& to_add);
		
	private:
		std::vector<TriggerInfo> triggers;
	};

	template<typename T>
	template<typename ...Types>
	inline TriggerBuilder<T>::TriggerBuilder(Types&& ... args)
		: to_build{ .trigger = T{std::forward<Types>(args)...} }
	{
	}
}