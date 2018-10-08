#pragma once
#include "../Input/Input.hpp"
#include "../Components/Renderer.hpp"
#include "../Class/Sound.hpp"
namespace Event
{
	struct TestEvents
	{
		static void BlendSelect(ECS::EntityManager& mana)
		{
			auto& layer1 = mana.getEntitiesByGroup(ENTITY_GROUP::LAYER1);
			for (auto& it : layer1)
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
				{
					Sound sound("onion");
					sound.play(false);
					it->getComponent<ECS::AlphaBlend>().blendMode = ECS::AlphaBlend::INVSRC;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
				{
					it->getComponent<ECS::AlphaBlend>().blendMode = ECS::AlphaBlend::ALPHA;
				}
			}
		}
	};
}