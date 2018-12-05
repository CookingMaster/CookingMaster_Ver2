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
		ResourceManager::GetGraph().load("Resource/image/cook.png", "tkg");
		ResourceManager::GetGraph().load("Resource/image/cook2.png", "tkg2");
		ResourceManager::GetGraph().load("Resource/image/cook3.png", "tkg3");
		//スライダー
		ResourceManager::GetGraph().load("Resource/image/slider.png", "slider");
		ResourceManager::GetGraph().load("Resource/image/slider_bar.png", "slider_bar");
		ResourceManager::GetGraph().load("Resource/image/1280.png", "back");
		//テーブル
		ResourceManager::GetGraph().load("Resource/image/table.png", "table");
		//本
		ResourceManager::GetGraph().load("Resource/image/menu_kari.png", "menu");
		//オプション
		ResourceManager::GetGraph().load("Resource/image/optionmenu.png", "option");
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
	}

	void StageSelect::initialize()
	{
		//ハイスコアの取得テスト
		score_ = ECS::ScoreArcheType::CreateSelectScoreEntity("font", Vec2{ 100,200 }, ECS::StageHighScore::STAGE2, *entityManager_)->
			getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE2);

		ECS::ArcheType::CreateGameBG("back",  Vec2{ 0.f,0.f},*entityManager_);

		ECS::ArcheType::CreateEntity("table", Vec2{ 0,0 },
			*entityManager_, ENTITY_GROUP::BACK)->getComponent<ECS::SpriteDraw>().setPivot(Vec2{0.f,0.f});

		auto book_ = ECS::ArcheType::CreateEntity("menu",  Vec2{ System::SCREEN_WIDIH / 2, System::SCREEN_HEIGHT * 0.6f },
			*entityManager_,  ENTITY_GROUP::BACK_OBJECT);
		option_ = ECS::ArcheType::CreateEntity("option", Vec2{ (float)System::SCREEN_WIDIH * 0.8, System::SCREEN_HEIGHT * 0.5f },
			*entityManager_, ENTITY_GROUP::BACK);
		
		
		
		auto bookPos = book_->getComponent<ECS::Position>();

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
			ResourceManager::GetSound().load("Resource/sound/act_bgm.wav","stage1",SoundType::BGM);
			name->add<std::string>("BGM_name", "stage1");
			ON_SCENE_CHANGE(SceneName::GAME, name.get(), StackPopFlag::POP, true);
		}
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "セレクト画面");
		DrawFormatString(0, 100, 0xffffffff, "%d", score_);
		
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetSound().remove("selectBGM");
	}

}// namespace StageSelect