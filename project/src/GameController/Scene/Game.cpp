#include "Game.h"
#include "SceneManager.hpp"
#include "../../ArcheType/ArcheType.hpp"
#include "../../Input/Input.hpp"
#include "../../Class/Sound.hpp"
#include "../../ArcheType/UIArcheType.hpp"
#include "../../ArcheType/ScoreArcheType.hpp"
#include "../../ArcheType/PlayerArcheType.hpp"
#include "../../Components/NoteStateTransition.hpp"
namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
	{
		if (parame != nullptr)
			name_ = (parame->get<std::string>("BGM_name"));

		msl_.loadMusicScoreData("Resource/sound/MUSIC/" + name_ + "/" + name_ + ".txt");
	}
	void Game::initialize()
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/Act_Chara2.png", "test", 48, 6, 8, 64, 64);
		ResourceManager::GetSound().load("Resource/sound/SE/onion.ogg", "onion", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().load("Resource/image/1280.png", "game_bg");
		ResourceManager::GetGraph().load("Resource/image/bar_empty.png", "bar_empty");
		ResourceManager::GetGraph().load("Resource/image/bar_full.png", "bar_full");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/clock.png", "clock");
		ResourceManager::GetGraph().load("Resource/image/needle.png", "needle");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "pause");

		//プレイヤーの画像読み込み
		ResourceManager::GetGraph().loadDiv("Resource/image/playerd.png", "player", 15, 3, 5, 500, 505);

		Sound s(name_);
		nc_.resetData(msl_.getBPM(), msl_.getBeat(), msl_.getOffsetTime());
		s.play(false,false);
		//背景
		ECS::ArcheType::CreateEntity("game_bg", Vec2(0.f, 0.f), *entityManager_, ENTITY_GROUP::BACK);
		//プレイヤー
		ECS::Player::CreatePlayer(
			name_,
			"player",
			Vec2(500.f, 505.f),
			Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f),
			msl_.getBPM(),
			msl_.getBeat(),
			*entityManager_);
		//スコアのバー
		ECS::UIArcheType::CreateEmptyBarUI("bar_empty", Vec2(431.f, 44.f), Vec2(0.f, 0.f), *entityManager_);
		ECS::UIArcheType::CreateFullBarUI("bar_full", Vec2(424.f, 38.f), Vec2(0.f, 0.f), msl_.getMaxPoint(), *entityManager_);
		//得点(パーセンテージ)表示
		ECS::UIArcheType::CreateFontUI("font", Vec2(25.f, 45.f), Vec2(50.f, 50.f), *entityManager_);
		//時計
		ECS::Entity* clock = ECS::UIArcheType::CreateClockUI("clock", Vec2(800.f, 100.f), *entityManager_);
		clock->getComponent<ECS::SimpleDraw>().doCenter(true);
		
		//時計の針
		ECS::UIArcheType::CreateNeedleUI("needle", Vec2(800.f, 100.f), *entityManager_, 1.f);
		//おやっさんを攻撃表示で召喚する
		boss_ = std::make_unique<BossController>(*entityManager_);
	}

	void Game::update()
	{
		entityManager_->update();
		//おやっさんテスト(Dキー押すと笑うよ)
		boss_->speekComb();
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
		nc_.run(msl_.getNotesData(), msl_.getScoreData(), *entityManager_);

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
	
	//ノーツ判定処理とスコアの取得
	[[nodiscard]]int Game::getNoteScore()
	{
		auto& input = Input::Get();
		//入力無し、同時押しは無視
		if ((input.getKeyFrame(KEY_INPUT_LEFT) == 1 && input.getKeyFrame(KEY_INPUT_RIGHT) == 1) ||
			(input.getKeyFrame(KEY_INPUT_LEFT) == 0 && input.getKeyFrame(KEY_INPUT_RIGHT) == 0))
		{
			return 0;
		}

		//入力方向を取得
		ECS::Direction::Dir dir = ECS::Direction::Dir::U;
		if (input.getKeyFrame(KEY_INPUT_LEFT) == 1)	 { dir = ECS::Direction::Dir::L; }
		if (input.getKeyFrame(KEY_INPUT_RIGHT) == 1) { dir = ECS::Direction::Dir::R; }

		auto& note = entityManager_->getEntitiesByGroup(ENTITY_GROUP::NOTE);
		for (auto& it : note)
		{
			auto& itnotestate = it->getComponent<ECS::NoteStateTransition>();

			if (itnotestate.isActiveNote())
			{
				//入力方向とノーツの向きが一致していない場合は無効
				if (itnotestate.getNoteDir() != dir)
				{
					break;
				}
				auto nowstate = itnotestate.getNoteState();

				//ノーツの状態を遷移
				itnotestate.ActionToChangeNoteState();
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