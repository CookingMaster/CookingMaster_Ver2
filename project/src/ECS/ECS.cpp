#include "ECS.hpp"

void ECS::Entity::addGroup(Group group) noexcept
{
	groupBitSet_[group] = true;
	manager_.addToGroup(this, group);
}

void ECS::EntitiesUpdate(const std::vector<Entity*>& entities)
{
	for (const auto& it : entities)
	{
		it->update();
	}
}

void ECS::EntitiesDraw2D(const std::vector<Entity*>& entities)
{
	for (const auto& it : entities)
	{
		it->draw2D();
	}
}

void ECS::EntitiesDraw3D(const std::vector<Entity*>& entities)
{
	for (const auto& it : entities)
	{
		it->draw3D();
	}
}
