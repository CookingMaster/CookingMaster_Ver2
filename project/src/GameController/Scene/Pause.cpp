#include "Pause.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"
#include "../../ArcheType/UIArcheType.hpp"


namespace Scene
{
	Pause::Pause(IOnSceneChangeCallback* scenePauseChange, [[maybe_unused]] const Parameter& parame, ECS::EntityManager* entityManager)
		: AbstractScene(scenePauseChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/pause_black.png", "pause_bg");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "slide");
		ResourceManager::GetGraph().load("Resource/image/button.png", "button");

		//背景
		ECS::UIArcheType::CreatePauseBG("pause_bg", Vec2{ 0.f,0.f }, *entityManager);
		//ポーズUIのスライド
		slide = ECS::UIArcheType::CreatePauseUI("slide", Vec2{ 522.f,255.f }, Vec2{ 640.f,360.f }, *entityManager);
		//ボタン
		button = ECS::UIArcheType::CreateButtonUI("button", Vec2{ 138.f, 56.f }, Vec2{ 470.f, 430.f }, *entityManager);
	}
	void Pause::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			Parameter param;
			param.add("non",true);
			__super::getCallBack().onSceneChange(SceneName::GAME, &param, StackPopFlag::NON);
			//BGM再開すること
			return;
		}

		//拡大が終わったら
		if (slide->getComponent<ECS::ExpandComponentSystem>().endFlag())
		{
			//ECS::UIArcheType::CreateButtonUI("continue", Vec2{138.f, 56.f}, Vec2{ 400.f, 400.f }, 0, *entityManager_);
		}

		if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1) {
			button->getComponent<ECS::ButtonCommponent>().setSelect(-1);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1) {
			button->getComponent<ECS::ButtonCommponent>().setSelect(1);
		}

	}
	void Pause::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "PAUSE画面");
	}

}