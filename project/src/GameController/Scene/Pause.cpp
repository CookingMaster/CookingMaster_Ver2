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

		name_ = parame->get<std::string>("BGM_name");
		bgmPath_ = parame->get<std::string>("BGM_path");
		bg_ = ECS::UIArcheType::CreatePauseBG("pause_bg", Vec2{ 0.f,0.f }, *entityManager);
		
		//size(522,255), pos(640,360):3‚Â‚Ìƒ{ƒ^ƒ“‚Ì‚¤‚¿¶‚Ì¶ã
		slide_ = ECS::UIArcheType::CreatePauseUI("slide", Vec2{ 522.f,255.f }, Vec2{ 640.f,360.f }, *entityManager);
		
		button_ = ECS::UIArcheType::CreateButtonUI("button", Vec2{ 138.f, 56.f }, Vec2{ 400.f, 397.f }, *entityManager);
		moji_ = ECS::UIArcheType::CreateButtonMojiUI("moji", Vec2{ 168.f, 56.f }, Vec2{ 400.f, 397.f }, *entityManager);
		frame_ = ECS::UIArcheType::CreateSelectFrame("frame", Vec2{ 400.f - 7.f, 397.f - 7.f }, *entityManager);
	}
	void Pause::update()
	{
		//entityManager_->update();
		button_->update();
		slide_->update();
		moji_->update();
		bg_->update();

		moveCursor();
		backToGame();
		selectButton();

	}
	void Pause::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	}

	Pause::~Pause()
	{
		button_->destroy();
		slide_->destroy();
		moji_->destroy();
		bg_->destroy();
		frame_->destroy();
	}


	void Pause::selectButton()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_Z)) {
			int select = frame_->getComponent<ECS::SelectFrame>().getSelect();
			auto bgm_name = std::make_unique<Parameter>();
			switch (select)
			{
			case 0:
			{
				DOUT << "Game Continue" << std::endl;
				bgm_name->add<std::string>("BGM_name", name_);
				Sound bgm(name_);
				bgm.play(false, false);
				ON_SCENE_CHANGE(SceneName::BACK_TO_SCENE, bgm_name.get(), StackPopFlag::POP, false);
				break;
			}
			case 1:
			{
				DOUT << "Restart" << std::endl;
				bgm_name->add<std::string>("BGM_name", name_);
				auto number = name_.back();
				bgm_name->add<size_t>("stageNum", static_cast<size_t>(atoi(&number)));
				bgm_name->add<std::string>("BGM_path", bgmPath_);
				ON_SCENE_CHANGE(SceneName::GAME, bgm_name.get(), StackPopFlag::ALL_CLEAR, true);
				break;
			}
			case 2:
			{
				DOUT << "Back To Select" << std::endl;
				Sound(name_).stop();
				ON_SCENE_CHANGE(SceneName::SELECT, nullptr, StackPopFlag::ALL_CLEAR, true);
				break;
			}
			}
		}
	}
	void Pause::backToGame()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", name_);
			Sound(name_).play(false, false);
			ON_SCENE_CHANGE(SceneName::BACK_TO_SCENE, bgm_name.get(), StackPopFlag::POP, false);
		}
	}

	void Pause::moveCursor()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) == 1) {
			frame_->getComponent<ECS::SelectFrame>().setSelect(-1);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) == 1) {
			frame_->getComponent<ECS::SelectFrame>().setSelect(1);
		}
	}

}