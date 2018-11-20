#include "Pause.h"
#include "../../Input/Input.hpp"
#include "SceneManager.hpp"
#include "../GameController.h"
#include "../../System/System.hpp"
#include "../../ArcheType/UIArcheType.hpp"
#include "../src/Class/Sound.hpp"

namespace Scene
{
	Pause::Pause(IOnSceneChangeCallback* scenePauseChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(scenePauseChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/pause_black.png", "pause_bg");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "slide");
		ResourceManager::GetGraph().load("Resource/image/button.png", "button");
		ResourceManager::GetGraph().load("Resource/image/pause_moji.png", "moji");		
		ResourceManager::GetGraph().load("Resource/image/button_frame.png", "frame");

		name = parame->get<std::string>("BGM_name");
		bg = ECS::UIArcheType::CreatePauseBG("pause_bg", Vec2{ 0.f,0.f }, *entityManager);
		
		//size(522,255), pos(640,360):3‚Â‚Ìƒ{ƒ^ƒ“‚Ì‚¤‚¿¶‚Ì¶ã
		slide = ECS::UIArcheType::CreatePauseUI("slide", Vec2{ 522.f,255.f }, Vec2{ 640.f,360.f }, *entityManager);
		
		button = ECS::UIArcheType::CreateButtonUI("button", Vec2{ 138.f, 56.f }, Vec2{ 400.f, 397.f }, *entityManager);
		moji = ECS::UIArcheType::CreateButtonMojiUI("moji", Vec2{ 168.f, 56.f }, Vec2{ 400.f, 397.f }, *entityManager);
		frame = ECS::UIArcheType::CreateSelectFrame("frame", Vec2{ 400.f - 3.f, 397.f - 3.f }, *entityManager);
	}
	void Pause::update()
	{
		//entityManager_->update();
		button->update();
		slide->update();
		moji->update();
		bg->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", name);
			Sound(name).play(false, false);
			ON_SCENE_CHANGE(SceneName::BACK_TO_SCENE, bgm_name.get(), StackPopFlag::POP, false);
		}

		if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1) {
			frame->getComponent<ECS::SelectFrame>().setSelect(-1);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1) {
			frame->getComponent<ECS::SelectFrame>().setSelect(1);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_Z)) {
			int select = frame->getComponent<ECS::SelectFrame>().getSelect();
			switch (select)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			}
		}
	}
	void Pause::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "PAUSE");
	}

	Pause::~Pause()
	{
		button->destroy();
		slide->destroy();
		moji->destroy();
		bg->destroy();
	}

}