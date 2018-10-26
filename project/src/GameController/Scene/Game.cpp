#include "Game.h"
#include "SceneManager.hpp"
#include "../../ArcheType/TestArcheType.hpp"
#include "../../Input/Input.hpp"
#include "../../Class/Sound.hpp"
#include "../../ArcheType/UIArcheType.hpp"
namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange),
		entityManager_(entityManager),
		playerDetail(parame)	//前のシーンの情報を取得
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/Act_Chara2.png", "test", 48, 6, 8, 64, 64);
		ResourceManager::GetSound().load("Resource/sound/onion.ogg", "onion", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().loadDiv("Resource/image/Chara_Test.png", "chara", 18, 6, 3, 64, 64);
		ResourceManager::GetGraph().load("Resource/image/bar_empty.png", "bar_empty");
		ResourceManager::GetGraph().load("Resource/image/bar_full.png", "bar_full");
		ResourceManager::GetGraph().load("Resource/image/clock.png", "clock");
		ResourceManager::GetGraph().load("Resource/image/needle.png", "needle");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetSound().load("Resource/sound/Let'sCooking.wav", "BGM", SoundType::BGM);
		if (playerDetail.get<std::string>("名前") == "たかし")
		{
			ECS::ArcheType::CreateTestEntity("test", Vec2{ 100.f,300.f }, *entityManager_);
		}
		if (playerDetail.get<std::string>("名前") == "まゆみ")
		{
			ECS::Entity* entity = ECS::ArcheType::CreateTestEntity("test", Vec2{ 100.f,300.f }, *entityManager_);
			entity->getComponent<ECS::Color>().blue = 0;
			entity->getComponent<ECS::Color>().green = 0;
		}
		if (playerDetail.get<std::string>("名前") == "みつひこ")
		{
			ECS::Entity* entity = ECS::ArcheType::CreateTestEntity("test", Vec2{ 100.f,300.f }, *entityManager_);
			entity->getComponent<ECS::AlphaBlend>().blendMode = ECS::AlphaBlend::INVSRC;
		}
		Sound s("BGM");
		msl.loadMusicScoreData("Resource/score/musicScoreTest.txt");
		nc.resetData(msl.GetBPM(), msl.GetOffsetTime());
		s.play(false,false);
		//プレイヤテスト
		ECS::ArcheType::CreatePlayerEntity("chara", "BGM", Vec2{ 300, 100 }, 20, *entityManager_);
		//バー
		ECS::UIArcheType::CreateEmptyBarUI("bar_empty", Vec2{ 431.f,44.f }, Vec2{ 300.f,300.f }, *entityManager_);
		bar = ECS::UIArcheType::CreateFullBarUI("bar_full", Vec2{ 424.f,38.f }, Vec2{ 300.f,300.f }, *entityManager_);
		//時計
		ECS::Entity* clock = ECS::UIArcheType::CreateClockUI("clock", Vec2{ 800.f,100.f }, *entityManager_);
		clock->getComponent<ECS::SimpleDraw>().doCenter(true);        
		needle = ECS::UIArcheType::CreateNeedleUI("needle", Vec2{ 800.f,100.f }, *entityManager_, 1.f);
		font = ECS::UIArcheType::CreateFontUI("font", Vec2{25.f, 45.f}, Vec2{ 450.f,350.f }, *entityManager_);
	}

	void Game::update()
	{
		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_A) == 1)
		{
			getCallBack().onSceneChange(SceneName::TITLE, nullptr, StackPopFlag::POP);
			return;
		}

		if (Input::Get().getKeyFrame(KEY_INPUT_V) == 1)
		{
			bar->getComponent<ECS::BarComponentSystemX>().addScore(43);
		}
		//フォント
		{
			int num = bar->getComponent<ECS::BarComponentSystemX>().getScore();
			font->getComponent<ECS::DrawFont>().setNumber(num);
			//font->getComponent<ECS::ExpandReduceComponentSystem>().
		}
		nc.run(msl.GetNotesData(), msl.GetScoreData(), *entityManager_);
	}

	void Game::draw()
	{
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		//グループ順に描画
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		SetDrawMode(DX_DRAWMODE_NEAREST);
		DrawFormatString(0, 0, 0xffffffff, "ゲーム画面");
		DrawFormatString(300, 50, 0xffffffff, "%s\n", playerDetail.get<std::string>("名前").c_str());
	}

	Game::~Game()
	{
		ResourceManager::GetGraph().removeDivGraph("test");
		ResourceManager::GetSound().remove("onion");
		ResourceManager::GetSound().remove("BGM");
		entityManager_->allDestory();
	}
}