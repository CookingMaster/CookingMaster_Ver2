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
			name = (parame->get<std::string>("BGM_name"));
	}
	void Game::initialize()
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/Act_Chara2.png", "test", 48, 6, 8, 64, 64);
		ResourceManager::GetSound().load("Resource/sound/onion.ogg", "onion", SoundType::SE);
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
		

		Sound s(name);
		msl.loadMusicScoreData("Resource/score/musicScoreTest.txt");
		nc.resetData(msl.GetBPM(), msl.GetOffsetTime());
		s.play(false,false);
		//背景
		ECS::ArcheType::CreateGameBG("game_bg", Vec2(0.f, 0.f), *entityManager_);
		//プレイヤテスト
		ECS::ArcheType::CreatePlayerEntity("chara", name.c_str(), Vec2(300.f, 100.f), 20, *entityManager_);
		//スコアのバー
		ECS::UIArcheType::CreateEmptyBarUI("bar_empty", Vec2(431.f, 44.f), Vec2(300.f, 300.f), *entityManager_);
		ECS::UIArcheType::CreateFullBarUI("bar_full", Vec2(424.f, 38.f), Vec2(300.f, 300.f), *entityManager_);
		//時計
		ECS::Entity* clock = ECS::UIArcheType::CreateClockUI("clock", Vec2(800.f, 100.f), *entityManager_);
		clock->getComponent<ECS::SimpleDraw>().isCenter(true);
		//時計の針
		ECS::UIArcheType::CreateNeedleUI("needle", Vec2(800.f, 100.f), *entityManager_, 1.f);
		//得点(パーセンテージ)表示
		ECS::UIArcheType::CreateFontUI("font", Vec2(25.f, 45.f), Vec2(450.f, 350.f), *entityManager_);
	}

	void Game::update()
	{
		//以下、仮の判定処理↓
		if (Input::Get().getKeyFrame(KEY_INPUT_SPACE) == 1)
		{
			for (auto& it : entityManager_->getEntitiesByGroup(ENTITY_GROUP::NOTE))
			{
				auto& itnotestate = it->getComponent<ECS::NoteStateTransition>();
				auto nowstate = itnotestate.getNoteState();

				if (itnotestate.ActionToChangeNoteState())
				{
					switch (nowstate)
					{
					case ECS::NoteState::State::BAD:
						DOUT << "BAD" << std::endl;
						break;
					case ECS::NoteState::State::GOOD:
						DOUT << "GOOD" << std::endl;
						break;
					case ECS::NoteState::State::GREAT:
						DOUT << "GREAT" << std::endl;
						break;
					case ECS::NoteState::State::PARFECT:
						DOUT << "PARFECT" << std::endl;
						break;
					}
					break;
				}
			}
		}
		//ここまで仮の判定処理↑


		entityManager_->update();
		if (Input::Get().getKeyFrame(KEY_INPUT_A) == 1)
		{
			getCallBack().onSceneChange(SceneName::TITLE, nullptr, StackPopFlag::POP, true);
			return;
		}

		if (Input::Get().getKeyFrame(KEY_INPUT_V) == 1)
		{
			for (auto& it : entityManager_->getEntitiesByGroup(ENTITY_GROUP::UI))
			{
				if (it->hasComponent<ECS::BarComponentSystemX>())
				{
					it->getComponent<ECS::BarComponentSystemX>().addScore(1);
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
		nc.run(msl.GetNotesData(), msl.GetScoreData(), *entityManager_);

		if (Input::Get().getKeyFrame(KEY_INPUT_C) == 1)
		{
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name",name);
			//BGM止めること
			Sound(name).stop();
			ON_SCENE_CHANGE(SceneName::PAUSE, bgm_name.get(), StackPopFlag::NON, true);
		}
		

		if (Input::Get().getKeyFrame(KEY_INPUT_RETURN) == 1) {
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", name);
			//BGM止めること
			Sound(name).stop();
			ON_SCENE_CHANGE(SceneName::RESULT, bgm_name.get(), StackPopFlag::POP, true);
		}
	}

	void Game::draw()
	{
		//グループ順に描画
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
		DrawFormatString(0, 0, 0xffffffff, "ゲーム画面");
		if (!name.empty())
		{
			DrawFormatString(0, 100, 0xffffffff, "%s", name.c_str());
		}
	}

	Game::~Game()
	{
		ResourceManager::GetGraph().removeDivGraph("test");
		ResourceManager::GetSound().remove("onion");
		ResourceManager::GetSound().remove(name);
		entityManager_->allDestory();
	}
	
}