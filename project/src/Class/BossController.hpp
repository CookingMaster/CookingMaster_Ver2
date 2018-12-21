/**
* @file BossController.hpp
* @brief おやっさん系エンティティをまとめて管理します
* @author tonarinohito
* @date 2018/12/21
*/
#pragma once
#include "../ArcheType/GameEffectsArcheType.hpp"


class BossController final
{
private:
	ECS::Entity* bossBody_ = nullptr;
	ECS::Entity* bossFace_ = nullptr;
	ECS::Entity* bossBalloon_ = nullptr;
	ECS::Entity* bossMessage_ = nullptr;
	ECS::EntityManager& manager_;
public:
	BossController(ECS::EntityManager manager) :
		manager_(manager)
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/chef.png","Boss",3,3,1,132,243);
		ResourceManager::GetGraph().loadDiv("Resource/image/chef_face.png", "BossFace", 3, 3, 1, 132, 243);//かり
		ResourceManager::GetGraph().load("Resource/image/number2.png", "number");//かり
		bossBody_ = ECS::GameEffectsArcheType::CreateBossBody("Boss", Vec2{100.f,100.f}, &manager_);
		bossFace_ = ECS::GameEffectsArcheType::CreateBossFace("BossFace", Vec2{ 100.f,100.f }, &manager_);
	}

	void speekComb(const int& num)
	{
		ECS::GameEffectsArcheType::CreateBossBalloon("balloon", Vec2{ 200,100 }, 120, &manager_);
		ECS::GameEffectsArcheType::CreateBossMessage("number", Vec2{ 200,100 }, ECS::Rectangle(0,0,50,50),num, 120, &manager_);
	}
};