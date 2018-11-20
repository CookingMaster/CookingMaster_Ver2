﻿#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"
namespace Scene
{
	void StageSelect::selectStageMove(ECS::Entity* pEntity)
	{
		auto& pos = pEntity->getComponent<ECS::Position>();
		if (Input::Get().getKeyFrame(KEY_INPUT_DOWN) == 1 &&
			pos.val.y <= stageMap["stage3"]->getComponent<ECS::Position>().val.y - UI_HEIGHT)
		{
			pos.val.y += UI_HEIGHT;
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_UP) == 1 &&
			pos.val.y >= stageMap["stage1"]->getComponent<ECS::Position>().val.y + UI_HEIGHT)
		{
			pos.val.y -= UI_HEIGHT;
		}
		
	}

	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/1280.png", "back");
		ResourceManager::GetGraph().load("Resource/image/book.png", "book");
		ResourceManager::GetGraph().load("Resource/image/point.png", "point");
		ResourceManager::GetGraph().load("Resource/image/stage.png", "stage");
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
	}

	void StageSelect::initialize()
	{
		score = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,100 }, ECS::StageHighScore::STAGE1, *entityManager_)->
			getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE1);

		ECS::ArcheType::CreateGameBG("back",  Vec2{ 0.f,0.f},*entityManager_);

		book = ECS::ArcheType::CreateEntity("book",  Vec2{ System::SCREEN_WIDIH / 2, System::SCREEN_HEIGHT * 0.6f },
			*entityManager_,  ENTITY_GROUP::BACK);
		auto bookPos = book->getComponent<ECS::Position>();

		stageMap["stage1"] = ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y - UI_HEIGHT * 2 }, *entityManager_, ENTITY_GROUP::UI);
		stageMap["stage2"] = ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y - UI_HEIGHT },  *entityManager_, ENTITY_GROUP::UI);
		stageMap["stage3"] = ECS::ArcheType::CreateEntity("stage", Vec2{ bookPos.val.x * 0.7f, bookPos.val.y },       *entityManager_, ENTITY_GROUP::UI);
		point = ECS::ArcheType::CreateEntity("point", Vec2{ bookPos.val.x * 0.4f, bookPos.val.y - UI_HEIGHT * 2 }, *entityManager_, ENTITY_GROUP::UI);

		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_X) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage1");
			ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav", "Stage1", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage2");
			ResourceManager::GetSound().load("Resource/sound/act_bgm.wav", "Stage2", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto stage_name = std::make_unique<Parameter>();
			stage_name->add<std::string>("BGM_name", "Stage3");
			ResourceManager::GetSound().load("Resource/sound/Grass.wav", "Stage3", SoundType::BGM);
			ON_SCENE_CHANGE(SceneName::GAME, stage_name.get(), StackPopFlag::POP, true);
		}
		
		selectStageMove(point);
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
		DrawFormatString(0, 100, 0xffffffff, "%d", score);
		
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}
}