#include "Game.h"
#include "SceneManager.hpp"
#include "../../ArcheType/TestArcheType.hpp"
#include "../../Input/Input.hpp"
#include "../../Class/Sound.hpp"
#include "../../ArcheType/UIArcheType.hpp"
#include "../../ArcheType/ScoreArcheType.hpp"
#include "../../Components/NoteStateTransition.hpp"
namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		if (parame != nullptr)
			name_ = (parame->get<std::string>("BGM_name"));
	}
	void Game::initialize()
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/Act_Chara2.png", "test", 48, 6, 8, 64, 64);
		ResourceManager::GetSound().load("Resource/sound/SE/onion.ogg", "onion", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().load("Resource/image/1280.png", "game_bg");
		ResourceManager::GetGraph().loadDiv("Resource/image/Chara_Test.png", "chara", 18, 6, 3, 64, 64);
		ResourceManager::GetGraph().load("Resource/image/bar_empty.png", "bar_empty");
		ResourceManager::GetGraph().load("Resource/image/bar_full.png", "bar_full");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/clock.png", "clock");
		ResourceManager::GetGraph().load("Resource/image/needle.png", "needle");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "pause");
		

		Sound s(name_);
		msl_.loadMusicScoreData("Resource/score/musicScoreTest.txt");
		nc_.resetData(msl_.GetBPM(), msl_.getOffsetTime());
		s.play(false,false);
		//背景
		ECS::ArcheType::CreateGameBG("game_bg", Vec2(0.f, 0.f), *entityManager_);
		//プレイヤテスト
		ECS::ArcheType::CreatePlayerEntity("chara", name_.c_str(), Vec2(300.f, 100.f), 20, *entityManager_);
		//スコアのバー
		ECS::UIArcheType::CreateEmptyBarUI("bar_empty", Vec2(431.f, 44.f), Vec2(300.f, 300.f), *entityManager_);
		ECS::UIArcheType::CreateFullBarUI("bar_full", Vec2(424.f, 38.f), Vec2(300.f, 300.f), msl_.GetMaxPoint(), *entityManager_);
		//時計
		ECS::UIArcheType::CreateFontUI("font", Vec2(25.f, 45.f), Vec2(450.f, 350.f), *entityManager_);
		//得点(パーセンテージ)表示
		ECS::Entity* clock = ECS::UIArcheType::CreateClockUI("clock", Vec2(800.f, 100.f), *entityManager_);
		clock->getComponent<ECS::SimpleDraw>().doCenter(true);
		
		//時計の針
		ECS::UIArcheType::CreateNeedleUI("needle", Vec2(800.f, 100.f), *entityManager_, 1.f);
	}

	void Game::update()
	{
		entityManager_->update();

		int score = getNoteScore();

		if (score > 0)
		{
			for (auto& it : entityManager_->getEntitiesByGroup(ENTITY_GROUP::UI))
			{
				if (it->hasComponent<ECS::BarComponentSystemX>())
				{
					it->getComponent<ECS::BarComponentSystemX>().addScore(score);
					num_ = it->getComponent<ECS::BarComponentSystemX>().getScore();
				}
				if (it->hasComponent<ECS::ExpandReduceComponentSystem>())
				{
					//スコアのフォント
					it->getComponent<ECS::ExpandReduceComponentSystem>().onExpand(true);
					it->getComponent<ECS::DrawFont>().setNumber(num_);
				}
			}
		}
		nc_.run(msl_.GetNotesData(), msl_.GetScoreData(), *entityManager_);

		if (Input::Get().getKeyFrame(KEY_INPUT_A) == 1)
		{
			getCallBack().onSceneChange(SceneName::TITLE, nullptr, StackPopFlag::POP, true);
			return;
		}
		changePauseScene();
		cangeResultScene();
	}

	void Game::draw()
	{
		//グループ順に描画
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "ゲーム画面");
		if (!name_.empty())
		{
			DrawFormatString(0, 100, 0xffffffff, "%s", name_.c_str());
		}
	}

	Game::~Game()
	{
		ResourceManager::GetGraph().removeDivGraph("test");
		ResourceManager::GetSound().remove("onion");
		ResourceManager::GetSound().remove(name_);
		entityManager_->allDestory();
	}
	
	//ノーツ判定処理
	[[nodiscard]]int Game::getNoteScore()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_SPACE) == 1)
		{
			auto& note = entityManager_->getEntitiesByGroup(ENTITY_GROUP::NOTE);
			for (auto& it : note)
			{
				auto& itnotestate = it->getComponent<ECS::NoteStateTransition>();
				auto nowstate = itnotestate.getNoteState();

				if (itnotestate.ActionToChangeNoteState())
				{
					switch (nowstate)
					{
					case ECS::NoteState::State::BAD:
						DOUT << "BAD" << std::endl;
						return 0;
					case ECS::NoteState::State::GOOD:
						DOUT << "GOOD" << std::endl;
						return 5;
					case ECS::NoteState::State::GREAT:
						DOUT << "GREAT" << std::endl;
						return 8;
					case ECS::NoteState::State::PARFECT:
						DOUT << "PARFECT" << std::endl;
						return 10;
					}
					break;
				}
			}
		}
		return 0;
	}

	//ポーズ画面遷移
	void Game::changePauseScene()
	{
		//ポーズ画面遷移
		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", name_);
			//BGMを停止する
			Sound(name_).stop();
			ON_SCENE_CHANGE(SceneName::PAUSE, bgm_name.get(), StackPopFlag::NON, true);
		}
	}

	//結果画面遷移
	void Game::cangeResultScene()
	{
		if (Input::Get().getKeyFrame(KEY_INPUT_RETURN) == 1) {
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", name_);
			//BGMを停止する
			Sound(name_).stop();
			ON_SCENE_CHANGE(SceneName::RESULT, bgm_name.get(), StackPopFlag::POP, true);
		}
	}
}