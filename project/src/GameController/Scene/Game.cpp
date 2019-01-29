#include "Game.h"
#include "SceneManager.hpp"
#include "../../ArcheType/ArcheType.hpp"
#include "../../Input/Input.hpp"
#include "../../Class/Sound.hpp"
#include "../../ArcheType/UIArcheType.hpp"
#include "../../ArcheType/ScoreArcheType.hpp"
#include "../../ArcheType/PlayerArcheType.hpp"
#include "../../Components/NoteStateTransition.hpp"
#include "../../Components/Renderer.hpp"
namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
		, bgmName_(parame->get<std::string>("BGM_name"))
		, bgmPath_(parame->get<std::string>("BGM_path"))
		, stageNum_(parame->get<size_t>("stageNum"))
	{
		//パラメーターに保存してある曲情報をもとに音楽ファイルと譜面を読み込む
		ResourceManager::GetSound().load(
			"Resource/sound/MUSIC/stage" + std::to_string(stageNum_) + "/" + bgmPath_,
			"stage" + std::to_string(stageNum_),
			SoundType::BGM);
		nc_.initialize(bgmName_,
			Vec2((System::SCREEN_WIDIH / 2.f) - 200.f, System::SCREEN_HEIGHT / 2.f),
			Vec2((System::SCREEN_WIDIH / 2.f) + 200.f, System::SCREEN_HEIGHT / 2.f));
		msl_.loadMusicScoreData("Resource/sound/MUSIC/" + bgmName_ + "/" + bgmName_ + ".txt");
	}
	void Game::initialize()
	{
		//Fade
		ResourceManager::GetGraph().load("Resource/image/fade_black.png", "fade");
		//鍋
		ResourceManager::GetGraph().loadDiv("Resource/image/bg_nabe1.png", "nabe1", 8, 8, 1, 140, 148);
		ResourceManager::GetSound().load("Resource/sound/SE/onion.ogg", "onion", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().load("Resource/image/bg_back.png", "bg_back");
		ResourceManager::GetGraph().load("Resource/image/bg_table.png", "bg_table");
		ResourceManager::GetGraph().load("Resource/image/mori1.png", "mori_full");
		ResourceManager::GetGraph().load("Resource/image/mori2.png", "mori_empty");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/test_font.png", "font");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "pause");

		//プレイヤーの画像読み込み
		ResourceManager::GetGraph().loadDiv("Resource/image/playerd.png", "player", 15, 3, 5, 500, 505);
		//マーカーの画像読み込み
		ResourceManager::GetGraph().load/*Div*/("Resource/image/note_marker.png", "marker"/*, 1, 1, 1, 200, 200*/);
		//スコア貼り紙
		ResourceManager::GetGraph().load("Resource/image/score_paper.png", "paper");

		//斬撃エフェクト(良)
		ResourceManager::GetGraph().loadDiv("Resource/image/cut_effect_d.png", "slash", 4, 4, 1, 384, 256);
		//斬撃エフェクト(悪)
		ResourceManager::GetGraph().loadDiv("Resource/image/cut_effect_bad_d.png", "slash_bad", 4, 4, 1, 256, 256);
		
		nc_.set(msl_.getBPM(), msl_.getBeat(), msl_.getOffsetTime());
		//背景
		ECS::ArcheType::CreateEntity("bg_back", Vec2(0.f, 0.f), *entityManager_, ENTITY_GROUP::BACK);
		ECS::ArcheType::CreateEntity("bg_table", Vec2(0.f, 193.f), *entityManager_, ENTITY_GROUP::BACK_OBJECT);
		//鍋
		ECS::GameEffectsArcheType::CreateSaucepan("nabe1", Vec2(431.f, 175.f), entityManager_);
		//ファン
		ResourceManager::GetGraph().load("Resource/image/bg_fan2.png", "fan");
		ECS::GameEffectsArcheType::CreateFan("fan", Vec2{ 1173.f, 96.f }, entityManager_);
		//プレイヤー
		ECS::Player::CreatePlayer(
			bgmName_,
			"player",
			Vec2(500.f, 505.f),
			Vec2(System::SCREEN_WIDIH / 2.f, (System::SCREEN_HEIGHT / 2.f) + 30),
			msl_.getBPM(),
			msl_.getBeat(),
			autoPerfectMode,
			*entityManager_);
		//スコア表示
		ECS::UIArcheType::CreateEmptyBarUI("mori_empty", Vec2(190.f, 136.f), Vec2(1074.f, 189.f), *entityManager_);
		ECS::UIArcheType::CreateFullBarUI("mori_full", Vec2(190.f, 136.f), Vec2(1074.f, 189.f), msl_.getMaxPoint(), *entityManager_);
		//スコア％表示用貼り紙
		ECS::ArcheType::CreateEntity("paper", Vec2{ 1030.f,370.f }, *entityManager_, ENTITY_GROUP::BACK_OBJECT);
		//得点(パーセンテージ)表示
		//ECS::UIArcheType::CreateFontUI("font", Vec2(25.f, 45.f), Vec2(50.f, 50.f), *entityManager_);
		//おやっさんを攻撃表示で召喚する
		boss_ = std::make_unique<BossController>(*entityManager_, msl_.getBPM(), msl_.getBeat(), bgmName_);
		//マーカー(左右に一つずつ)
		ECS::GameEffectsArcheType::CreateMarker("marker", bgmName_, msl_.getBPM(), msl_.getBeat(), ECS::Direction::Dir::L,
			Vec2((System::SCREEN_WIDIH / 2.f) - 200.f, System::SCREEN_HEIGHT / 2.f), entityManager_);
		ECS::GameEffectsArcheType::CreateMarker("marker", bgmName_, msl_.getBPM(), msl_.getBeat(), ECS::Direction::Dir::R,
			Vec2((System::SCREEN_WIDIH / 2.f) + 200.f, System::SCREEN_HEIGHT / 2.f), entityManager_);

		fade_ = ECS::ArcheType::CreateEntity
		(
			"fade",
			Vec2{ 0.f,0.f },
			*entityManager_,
			ENTITY_GROUP::TOP_FADE
		);
	}

	void Game::update()
	{
		auto fade = entityManager_->getEntitiesByGroup(ENTITY_GROUP::TOP_FADE);
		auto backs = entityManager_->getEntitiesByGroup(ENTITY_GROUP::BACK_OBJECT);
		if (!isPlay_)
		{
			for (auto& it : fade)
			{
				it->getComponent<ECS::AlphaBlend>().alpha -= 6;
			}
			for (auto& it : backs)
			{
				it->update();
			}
		}
			 
		if (!isPlay_ && fade[0]->getComponent<ECS::AlphaBlend>().alpha <= 0)
		{
			isPlay_ = true;
			//曲の再生
			Sound s(bgmName_);
			if (!s.isPlay())
			{
				s.play(false, false);
			}
		}
		if (isPlay_)
		{
			entityManager_->update();
			//おやっさんにコンボを入れる
			boss_->speekComb(comb_);
			int score = getNoteScore();

			if (score > 0)
			{
				for (auto& it : entityManager_->getEntitiesByGroup(ENTITY_GROUP::KITCHENWARE))
				{
					if (it->hasComponent<ECS::BarComponentSystemY>())
					{
						it->getComponent<ECS::BarComponentSystemY>().addScore(score);
						scoreNum_ = it->getComponent<ECS::BarComponentSystemY>().getScore();
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
			//デバッグ関数------------------------------------------------------------------------------
			if (Input::Get().getKeyFrame(KEY_INPUT_RETURN) == 1)
			{
				auto sendParame = std::make_unique<Parameter>();
				sendParame->add<std::string>("BGM_name", bgmName_);
				sendParame->add<int>("score", scoreNum_);
				sendParame->add<int>("maxcombo", maxComb_);
				//BGMを停止する
				Sound(bgmName_).stop();
				ON_SCENE_CHANGE(SceneName::RESULT, sendParame.get(), StackPopFlag::POP, true);
			}
			//-----------------------------------------------------------------------------------------

			changeResultScene();
			changePauseScene();
			saveMaxComb();
		}
		
	}

	void Game::draw()
	{
		//グループ順に描画
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	}

	Game::~Game()
	{
		ResourceManager::GetSound().remove("onion");
		ResourceManager::GetSound().remove(bgmName_);
		entityManager_->removeAll();
	}
	
	//ノーツ判定処理とスコアの取得
	[[nodiscard]]int Game::getNoteScore()
	{
		auto& input = Input::Get();
		auto& note = entityManager_->getEntitiesByGroup(ENTITY_GROUP::NOTE);

		//オートモードがオンのときの処理
		if (autoPerfectMode)
		{
			for (auto& it : note)
			{
				auto& itnotestate = it->getComponent<ECS::NoteStateTransition>();

				if (itnotestate.isActiveNote())
				{
					Sound se(itnotestate.getSEName());

					auto nowstate = itnotestate.getNoteState();
					switch (nowstate)
					{
					case ECS::NoteState::State::PARFECT:
						auto& player = entityManager_->getEntitiesByGroup(ENTITY_GROUP::GIRL)[0]->getComponent<ECS::PlayerController>();
						player.playSlashAnim(itnotestate.getNoteDir());

						//ノーツの状態を遷移
						itnotestate.ActionToChangeNoteState();
						
						DOUT << "PARFECT" << std::endl;
						se.play(false, true);
						++comb_; 
						ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_, ECS::AlphaBlend::BlendMode::SUB);
						return msl_.getPoint(nowstate, comb_);
					}
					break;
				}
			}
			return 0;
		}

		//入力無し、同時押し時はMISSのノーツのみ調べる
		if ((input.getKeyFrame(KEY_INPUT_LEFT) == 1 && input.getKeyFrame(KEY_INPUT_RIGHT) == 1) ||
			(input.getKeyFrame(KEY_INPUT_LEFT) == 0 && input.getKeyFrame(KEY_INPUT_RIGHT) == 0))
		{
			for (auto& it : note)
			{
				auto notestate = it->getComponent<ECS::NoteStateTransition>().getNoteState();
				if (notestate == ECS::NoteState::State::MISS)
				{
					DOUT << "MISS" << std::endl;
					comboReset();
				}
			}
			return 0;
		}

		//入力方向を取得
		ECS::Direction::Dir dir = ECS::Direction::Dir::U;
		if (input.getKeyFrame(KEY_INPUT_LEFT) == 1)	 { dir = ECS::Direction::Dir::L; }
		if (input.getKeyFrame(KEY_INPUT_RIGHT) == 1) { dir = ECS::Direction::Dir::R; }

		for (auto& it : note)
		{
			auto& itnotestate = it->getComponent<ECS::NoteStateTransition>();

			if (itnotestate.isActiveNote())
			{
				Sound se(itnotestate.getSEName());
				//入力方向とノーツの向きが一致していない場合は無効
				if (itnotestate.getNoteDir() != dir)
				{
					break;
				}

				auto nowstate = itnotestate.getNoteState();
				//ノーツの状態を遷移
				itnotestate.ActionToChangeNoteState();

				//コンボ数から追加ポイントを計算
				int score = msl_.getPoint(nowstate, comb_);
				switch (nowstate)
				{
				case ECS::NoteState::State::MISS:
					DOUT << "MISS" << std::endl;
					comboReset();
					return score;
				case ECS::NoteState::State::BAD:
					DOUT << "BAD" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash_bad", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					comboReset();
					return score;
				case ECS::NoteState::State::GOOD:
					DOUT << "GOOD" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					++comb_;
					return score;
				case ECS::NoteState::State::GREAT:
					DOUT << "GREAT" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					++comb_;
					return score;
				case ECS::NoteState::State::PARFECT:
					DOUT << "PARFECT" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					++comb_;
					return score;
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
			bgm_name->add<std::string>("BGM_path", bgmPath_);
			//BGMを停止する
			Sound(bgmName_).stop();
			ON_SCENE_CHANGE(SceneName::PAUSE, bgm_name.get(), StackPopFlag::NON, true);
		}
	}

	//結果画面遷移
	void Game::changeResultScene()
	{
		Sound sound(bgmName_);
		if (sound.getTotalTime() <= sound.getCurrentTime()) {
			auto sendParame = std::make_unique<Parameter>();
			sendParame->add<std::string>("BGM_name", bgmName_);
			sendParame->add<int>("score", scoreNum_);
			sendParame->add<int>("maxcombo", maxComb_);
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
			ON_SCENE_CHANGE(SceneName::RESULT, sendParame.get(), StackPopFlag::POP, true);
		}
	}

	//コンボを0にしておやっさんを怒らせる
	void Game::comboReset()
	{
		comb_ = 0;
		//↓おやっさんを怒らせる処理
		boss_->angry();
	}
	void Game::saveMaxComb()
	{
		if (maxComb_ < comb_)
		{
			maxComb_ = comb_;
		}
	}
}