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
		, bgmName_(parame->get<std::string>("BGM_name"))
		, stageNum_(parame->get<size_t>("stageNum"))
		, nc_(bgmName_)
	{
		msl_.loadMusicScoreData("Resource/sound/MUSIC/" + bgmName_ + "/" + bgmName_ + ".txt");
	}
	void Game::initialize()
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/Act_Chara2.png", "test", 48, 6, 8, 64, 64);
		ResourceManager::GetSound().load("Resource/sound/SE/onion.ogg", "onion", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().load("Resource/image/bg_back.png", "bg_back");
		ResourceManager::GetGraph().load("Resource/image/bg_table.png", "bg_table");
		ResourceManager::GetGraph().load("Resource/image/bar_empty.png", "bar_empty");
		ResourceManager::GetGraph().load("Resource/image/bar_full.png", "bar_full");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/clock.png", "clock");
		ResourceManager::GetGraph().load("Resource/image/needle.png", "needle");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "pause");

		//プレイヤーの画像読み込み
		ResourceManager::GetGraph().loadDiv("Resource/image/playerd.png", "player", 15, 3, 5, 500, 505);

		Sound s(bgmName_);
		nc_.set(msl_.getBPM(), msl_.getBeat(), msl_.getOffsetTime());
		s.play(false,false);
		//背景
		ECS::ArcheType::CreateEntity("bg_back", Vec2(0.f, 0.f), *entityManager_, ENTITY_GROUP::BACK);
		ECS::ArcheType::CreateEntity("bg_table", Vec2(0.f, 193.f), *entityManager_, ENTITY_GROUP::BACK);
		//プレイヤー
		ECS::Player::CreatePlayer(
			bgmName_,
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
		//おやっさんを攻撃表示で召喚する
		boss_ = std::make_unique<BossController>(*entityManager_);
		//曲の再生
		s.play(false, false);
	}

	void Game::update()
	{
		entityManager_->update();
		//おやっさんテスト(Dキー押すと笑うよ)
		boss_->speekComb(comb_);
		int score = getNoteScore();

		if (score > 0)
		{
			for (auto& it : entityManager_->getEntitiesByGroup(ENTITY_GROUP::UI))
			{
				if (it->hasComponent<ECS::BarComponentSystemX>())
				{
					it->getComponent<ECS::BarComponentSystemX>().addScore(score);
					scoreNum_ = it->getComponent<ECS::BarComponentSystemX>().getScore();
				}
				if (it->hasComponent<ECS::ExpandReduceComponentSystem>())
				{
					//スコアのフォント
					it->getComponent<ECS::ExpandReduceComponentSystem>().onExpand(true);
					it->getComponent<ECS::DrawFont>().setNumber(scoreNum_);
				}
			}
		}
		nc_.run(msl_.getNotesData(), msl_.getScoreData(), *entityManager_);

		if (Input::Get().getKeyFrame(KEY_INPUT_A) == 1)
		{
			getCallBack().onSceneChange(SceneName::TITLE, nullptr, StackPopFlag::POP, true);
			return;
		}
		changeResultScene();
		changePauseScene();
	}

	void Game::draw()
	{
		//グループ順に描画
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	}

	Game::~Game()
	{
		ResourceManager::GetGraph().removeDivGraph("test");
		ResourceManager::GetSound().remove("onion");
		ResourceManager::GetSound().remove(bgmName_);
		entityManager_->allDestory();
	}
	
	//ノーツ判定処理とスコアの取得
	[[nodiscard]]int Game::getNoteScore()
	{
		auto& input = Input::Get();
		//入力無し、同時押し時はMISSのノーツのみ調べる
		if ((input.getKeyFrame(KEY_INPUT_LEFT) == 1 && input.getKeyFrame(KEY_INPUT_RIGHT) == 1) ||
			(input.getKeyFrame(KEY_INPUT_LEFT) == 0 && input.getKeyFrame(KEY_INPUT_RIGHT) == 0))
		{
			auto& note = entityManager_->getEntitiesByGroup(ENTITY_GROUP::NOTE);
			for (auto& it : note)
			{
				auto notestate = it->getComponent<ECS::NoteStateTransition>().getNoteState();
				if (notestate == ECS::NoteState::State::MISS)
				{
					DOUT << "MISS" << std::endl;
					ComboReset();
				}
			}
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
				Sound se("onion");
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
				case ECS::NoteState::State::MISS:
					DOUT << "MISS" << std::endl;
					ComboReset();
					return 0;
				case ECS::NoteState::State::BAD:
					DOUT << "BAD" << std::endl;
					ComboReset();
					return 0;
				case ECS::NoteState::State::GOOD:
					DOUT << "GOOD" << std::endl;
					se.play(false, true);
					++comb_;
					return 5;
				case ECS::NoteState::State::GREAT:
					DOUT << "GREAT" << std::endl;
					se.play(false, true);
					++comb_;
					return 8;
				case ECS::NoteState::State::PARFECT:
					DOUT << "PARFECT" << std::endl;
					se.play(false, true);
					++comb_;
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
			bgm_name->add<std::string>("BGM_name", bgmName_);
			//BGMを停止する
			Sound(bgmName_).stop();
			ON_SCENE_CHANGE(SceneName::PAUSE, bgm_name.get(), StackPopFlag::NON, true);
		}
	}

	//結果画面遷移
	void Game::changeResultScene()
	{
		Sound sound(bgmName_);
		if (!sound.isPlay()) {
			auto bgm_name = std::make_unique<Parameter>();
			bgm_name->add<std::string>("BGM_name", bgmName_);
			//BGMを停止する
			Sound(bgmName_).stop();
			switch (stageNum_)
			{
			case 1:
				ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE1, scoreNum_, *entityManager_);
				break;
			case 2:
				ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE2, scoreNum_, *entityManager_);
				break;
			case 3:
				ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE3, scoreNum_, *entityManager_);
				break;
			}
			ON_SCENE_CHANGE(SceneName::RESULT, bgm_name.get(), StackPopFlag::POP, true);
		}
	}

	//コンボを0にしておやっさんを怒らせる
	void Game::ComboReset()
	{
		comb_ = 0;
		//↓おやっさんを怒らせる処理
	}
}