#pragma once

#include "GHTN.h"

#include "World.h"

namespace GHTN
{
	template<typename T>
	concept Effecting = requires(T effect, World& world)
	{
		effect(world);
	};

	template<Effecting ... Ts>
	using EffectingVariant = std::variant<Ts...>;
}
