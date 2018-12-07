#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"

namespace Scene
{
	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetSound().load("Resource/sound/onion.ogg", "onion", SoundType::SE);
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		//仮料理画像とUI
		ResourceManager::GetGraph().load("Resource/image/tamagokake.png", "tkg");
		ResourceManager::GetGraph().loadDiv("Resource/image/stage_kari.png", "stage",3,1,3,182,89);
		//スライダー
		ResourceManager::GetGraph().load("Resource/image/slider.png", "slider");
		ResourceManager::GetGraph().load("Resource/image/slider_bar.png", "slider_bar");
		ResourceManager::GetGraph().load("Resource/image/1280.png", "back");
		//テーブル
		ResourceManager::GetGraph().load("Resource/image/table.png", "table");
		//本
		ResourceManager::GetGraph().load("Resource/image/menu.png", "menu");
		//オプション
		ResourceManager::GetGraph().load("Resource/image/optionmenu.png", "option");
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
	}

	void StageSelect::initialize()
	{
		ECS::ArcheType::CreateEntity("table", Vec2{ 0.f,0.f },
			*entityManager_, ENTITY_GROUP::BACK)->getComponent<ECS::SpriteDraw>().setPivot(Vec2{0.f,0.f});

		ECS::ArcheType::CreateEntity("menu",  Vec2{ 84.f,134.f},*entityManager_, ENTITY_GROUP::BACK_OBJECT);

		option_ = ECS::ArcheType::CreateEntity("option", Vec2{ (float)System::SCREEN_WIDIH * 0.8, System::SCREEN_HEIGHT * 0.5f },
			*entityManager_, ENTITY_GROUP::BACK);
		option_->getComponent<ECS::SpriteDraw>().doCenter();
		
		ECS::ArcheType::CreateEntity("tkg", Vec2{ 692.f, 244.f },*entityManager_, ENTITY_GROUP::BACK_OBJECT);
		
		ECS::ArcheType::CreateEntity("tkg", Vec2{ 692.f, 244.f },*entityManager_, ENTITY_GROUP::BACK_OBJECT);

		ECS::ArcheType::CreateMultiEntity("stage", Vec2{ 243.f, 332.f },
			*entityManager_, ENTITY_GROUP::BACK_OBJECT)->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);

		ECS::ArcheType::CreateMultiEntity("stage", Vec2{ 243.f, 332.f + 89.f },
			*entityManager_, ENTITY_GROUP::BACK_OBJECT)->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);

		ECS::ArcheType::CreateMultiEntity("stage", Vec2{ 243.f, 332.f + 89.f * 2 },
			*entityManager_, ENTITY_GROUP::BACK_OBJECT)->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_A) == 1)
		{
			option_->changeGroup(ENTITY_GROUP::UI);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			auto name = std::make_unique<Parameter>();
			ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav","stage1",SoundType::BGM);
			name->add<std::string>("BGM_name", "stage1");
			ON_SCENE_CHANGE(SceneName::GAME, name.get(), StackPopFlag::POP, true);
		}
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}

}// namespace StageSelect