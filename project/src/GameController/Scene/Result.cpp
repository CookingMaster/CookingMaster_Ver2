#include "Result.h"
#include "../../System/System.hpp"
#include "../GameController.h"
#include "../../ArcheType/ResultArcheType.hpp"
#include "../../Components/ResultComponent.hpp"
#include "../../Components/UIComponents.hpp"

Scene::Result::Result(IOnSceneChangeCallback * sceneTitleChange, [[maybe_unused]]Parameter * parame, ECS::EntityManager * entityManager)
	:
	AbstractScene(sceneTitleChange)
	, entityManager_(entityManager)
{
}

void Scene::Result::initialize()
{
	ResourceManager::GetGraph().load("Resource/image/cloche.png", "cloche");
	ResourceManager::GetGraph().load("Resource/image/1280.png", "game_bg");
	ResourceManager::GetGraph().load("Resource/image/dish.png", "dish");

	counter_.setEndTime(300, 0);
	cloche_ = ECS::ResultArcheType::CreateClocheEntity("cloche", Vec2{ 600,400 }, *entityManager_);
	back_ = ECS::ResultArcheType::CreateBackEntity("game_bg", Vec2{ 0,0 }, *entityManager_);
	dish_ = ECS::ResultArcheType::CreateDishEntity("dish", Vec2{ 400,250 } , Vec2{ 600,425 }, *entityManager_);
}

void Scene::Result::update()
{
	counter_.add();
	entityManager_->update();

	if (counter_.getCurrentCount() >= 100) {
		cloche_->addComponent<ECS::FlyAway>(Vec2{ 1920,600 }, 3.f);
	}
	if (counter_.getCurrentCount() >= 110) {
		dish_->addComponent<ECS::Expand>(Vec2{ 1.5f,1.5f }, Easing::ExpoIn, 20.f);
	}

	if (counter_.isMax()) {
		DOUT << "BackToTitle" << std::endl;
		ON_SCENE_CHANGE(SceneName::TITLE, nullptr, StackPopFlag::ALL_CLEAR, true);
	}
}

void Scene::Result::draw()
{
	entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	DrawFormatString(0, 0, 0xffffffff, "RESULT");
}

Scene::Result::~Result()
{
	back_->destroy();
	cloche_->destroy();
	dish_->destroy();
}
