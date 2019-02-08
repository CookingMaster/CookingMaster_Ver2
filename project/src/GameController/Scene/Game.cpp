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
		, autoPerfectMode_(parame->get<bool>("autoFlag"))
	{
		//パラメーターに保存してある曲情報をもとに音楽ファイルと譜面を読み込む
		ResourceManager::GetSound().load(
			"Resource/sound/MUSIC/stage" + std::to_string(stageNum_) + "/" + bgmPath_,
			"stage" + std::to_string(stageNum_),
			SoundType::BGM);
		nc_.initialize(bgmName_,
			Vec2((System::SCREEN_WIDTH / 2.f) - 200.f, System::SCREEN_HEIGHT / 2.f),
			Vec2((System::SCREEN_WIDTH / 2.f) + 200.f, System::SCREEN_HEIGHT / 2.f));
		msl_.loadMusicScoreData("Resource/sound/MUSIC/" + bgmName_ + "/" + bgmName_ + ".txt");
	}
	void Game::initialize()
	{
		//Fade
		ResourceManager::GetGraph().load("Resource/image/fade_black.png", "fade");
		//鍋
		ResourceManager::GetGraph().loadDiv("Resource/image/bg_nabe1.png", "nabe1", 8, 8, 1, 140, 148);
		ResourceManager::GetGraph().loadDiv("Resource/image/bg_nabe2.png", "nabe2", 8, 8, 1, 186, 98);
		//鍋かんかん
		ResourceManager::GetSound().load("Resource/sound/SE/nabeHit.ogg", "nabeHit", SoundType::SE);
		//BPMアニメーションテストのため仮読み込み
		ResourceManager::GetGraph().load("Resource/image/bg_back.png", "bg_back");
		ResourceManager::GetGraph().load("Resource/image/rakugaki.png", "rakugaki");
		ResourceManager::GetGraph().load("Resource/image/bg_table.png", "bg_table");
		ResourceManager::GetGraph().load("Resource/image/mori1.png", "mori_full");
		ResourceManager::GetGraph().load("Resource/image/mori2.png", "mori_empty");
		ResourceManager::GetGraph().load("Resource/image/pause_.png", "pause");
		//CookingStartUI
		ResourceManager::GetGraph().load("Resource/image/gameStart/gameStart.png", "start");
		ResourceManager::GetGraph().loadDiv("Resource/image/gameStart/gong.png", "gong", 2, 2, 1, 150, 200);
		ResourceManager::GetGraph().loadDiv("Resource/image/gameStart/star.png", "star", 2, 2, 1, 55, 53);
		//FINISH
		ResourceManager::GetGraph().load("Resource/image/finish.png", "finish");

		//プレイヤーの画像読み込み
		ResourceManager::GetGraph().loadDiv("Resource/image/playerd.png", "player", 15, 3, 5, 500, 505);
		//マーカーの画像読み込み
		ResourceManager::GetGraph().load("Resource/image/note_marker.png", "marker");
		//スコア貼り紙
		ResourceManager::GetGraph().load("Resource/image/paper.png", "paper");
		//研修中
		ResourceManager::GetGraph().load("Resource/image/training.png", "training");
		//斬ったときの評価フォント
		ResourceManager::GetGraph().loadDiv("Resource/image/hyouka_font.png", "rank", 4, 1, 4, 256, 64);
		//斬撃エフェクト(良)
		ResourceManager::GetGraph().loadDiv("Resource/image/cut_effect_d.png", "slash", 4, 4, 1, 384, 256);
		//斬撃エフェクト(悪)
		ResourceManager::GetGraph().loadDiv("Resource/image/cut_effect_bad_d.png", "slash_bad", 4, 4, 1, 256, 256);
		//グチャ
		ResourceManager::GetSound().load("Resource/sound/SE/miss.ogg", "miss", SoundType::SE);
		//グチャ画像
		ResourceManager::GetGraph().loadDiv("Resource/image/dirty.png", "dirty", 3, 3, 1, 128, 75);

		//BAD時の音
		ResourceManager::GetSound().load("Resource/sound/SE/suka.ogg", "suka", SoundType::SE);

		nc_.set(msl_.getBPM(), msl_.getBeat(), msl_.getOffsetTime());
		//背景
		ECS::ArcheType::CreateEntity("bg_back", Vec2(0.f, 0.f), *entityManager_, ENTITY_GROUP::BACK);
		ECS::ArcheType::CreateEntity("bg_table", Vec2(0.f, 193.f), *entityManager_, ENTITY_GROUP::BACK_OBJECT);
		ECS::ArcheType::CreateEntity("rakugaki", Vec2{ 0.f,-28.f }, *entityManager_, ENTITY_GROUP::BACK_OBJECT);
		//鍋
		ECS::GameEffectsArcheType::CreateSaucepan("nabe1", Vec2(431.f, 175.f), entityManager_);
		auto nabe2 = ECS::GameEffectsArcheType::CreateSaucepan("nabe2", Vec2{ 720.f,217.f }, entityManager_);
		nabe2->getComponent<ECS::Animator>().changeFrame(5);
		//ファン
		ResourceManager::GetGraph().load("Resource/image/bg_fan2.png", "fan");
		ECS::GameEffectsArcheType::CreateFan("fan", Vec2{ 1173.f, 96.f }, entityManager_);
		//プレイヤー
		ECS::Player::CreatePlayer(
			bgmName_,
			"player",
			Vec2(500.f, 505.f),
			Vec2(System::SCREEN_WIDTH / 2.f, (System::SCREEN_HEIGHT / 2.f) + 30),
			msl_.getBPM(),
			msl_.getBeat(),
			autoPerfectMode_,
			*entityManager_);
		//スコア表示
		ECS::UIArcheType::CreateEmptyBarUI("mori_empty", Vec2(256.f, 183.f), Vec2(1041.f, 142.f), *entityManager_);
		ECS::UIArcheType::CreateFullBarUI("mori_full", Vec2(190.f, 136.f), Vec2(1074.f, 189.f), msl_.getMaxPoint(), *entityManager_);
		//スコア％表示用貼り紙
		ResourceManager::GetGraph().load("Resource/image/score_font2.png", "score_font");
		ECS::ArcheType::CreateEntity("paper", Vec2{ 1030.f,370.f }, *entityManager_, ENTITY_GROUP::BACK_OBJECT);
		if (autoPerfectMode_)
		{
			//オートモードでは研修中の貼り紙表示
			ECS::ArcheType::CreateEntity("training", Vec2{ 0.f,0.f }, *entityManager_, ENTITY_GROUP::KITCHENWARE);
		}
		else
		{
			//ノーマルプレイではスコアフォントを表示
			scoreFont_ = ECS::UIArcheType::CreateFontUI("score_font", Vec2{ 32.f, 64.f }, Vec2{ 1143.f,450.f }, *entityManager_);
		}
		//おやっさんを攻撃表示で召喚する
		boss_ = std::make_unique<BossController>(*entityManager_, msl_.getBPM(), msl_.getBeat(), bgmName_);
		fade_ = ECS::ArcheType::CreateEntity
		(
			"fade",
			Vec2{ 0.f,0.f },
			*entityManager_,
			ENTITY_GROUP::TOP_FADE
		);

		finishCnt_.setCounter(0, 1, 0, 60);
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
			if (isStartUIEnd()) {
				isPlay_ = true;
				//曲の再生
				Sound s(bgmName_);
				if (!s.isPlay())
				{
					s.play(false, false);
				}
				//マーカーを作成(左右に一つずつ)
				ECS::GameEffectsArcheType::CreateMarker("marker", bgmName_, msl_.getBPM(), msl_.getBeat(), ECS::Direction::Dir::L,
					Vec2((System::SCREEN_WIDTH / 2.f) - 200.f, System::SCREEN_HEIGHT / 2.f), entityManager_);
				ECS::GameEffectsArcheType::CreateMarker("marker", bgmName_, msl_.getBPM(), msl_.getBeat(), ECS::Direction::Dir::R,
					Vec2((System::SCREEN_WIDTH / 2.f) + 200.f, System::SCREEN_HEIGHT / 2.f), entityManager_);
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
				}
				if (!autoPerfectMode_)
				{
					scoreFont_->getComponent<ECS::ExpandReduceComponentSystem>().onExpand(true);
					scoreFont_->getComponent<ECS::DrawFont>().setNumber(scoreNum_);
				}
			}
			nc_.run(msl_.getNotesData(), msl_.getScoreData(), *entityManager_);

#ifdef _DEBUG
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
				sendParame->add<bool>("fullcombo", maxComb_ == msl_.getMaxNoteNum());
				//BGMを停止する
				Sound(bgmName_).stop();
				//オートモードは記録しない
				if (!autoPerfectMode_)
				{
					switch (stageNum_)
					{
					case 1:
						sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE1, scoreNum_, *entityManager_)
							->getComponent<ECS::ScoreSystem>().isUpdateScore());
						break;
					case 2:
						sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE2, scoreNum_, *entityManager_)
							->getComponent<ECS::ScoreSystem>().isUpdateScore());
						break;
					case 3:
						sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE3, scoreNum_, *entityManager_)
							->getComponent<ECS::ScoreSystem>().isUpdateScore());
						break;
					}
				}
				else
				{
					sendParame->add<bool>("newrecord", false);
				}
				ON_SCENE_CHANGE(SceneName::RESULT, sendParame.get(), StackPopFlag::POP, true);
			}
			//-----------------------------------------------------------------------------------------
#endif
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
		if (autoPerfectMode_)
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
					case ECS::NoteState::State::AUTO:
						auto& player = entityManager_->getEntitiesByGroup(ENTITY_GROUP::GIRL)[0]->getComponent<ECS::PlayerController>();
						player.playSlashAnim(itnotestate.getNoteDir());

						//ノーツの状態を遷移
						itnotestate.ActionToChangeNoteState();
						createRankFont(0);
						DOUT << "PARFECT" << std::endl;
						se.play(false, true);
						++comb_;
						ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
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
		if (input.getKeyFrame(KEY_INPUT_LEFT) == 1) { dir = ECS::Direction::Dir::L; }
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
					{
						Sound suka("suka");
						suka.play(false, true);
					}
					createRankFont(3);
					return score;
				case ECS::NoteState::State::GOOD:
					DOUT << "GOOD" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					createRankFont(2);
					++comb_;
					return score;
				case ECS::NoteState::State::GREAT:
					DOUT << "GREAT" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					createRankFont(1);
					++comb_;
					return score;
				case ECS::NoteState::State::PARFECT:
				case ECS::NoteState::State::AUTO:
					DOUT << "PARFECT" << std::endl;
					ECS::GameEffectsArcheType::CreateSlashEffect("slash", itnotestate.getPos(), itnotestate.getNoteDir(), entityManager_);
					se.play(false, true);
					createRankFont(0);
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
			auto send_parameter = std::make_unique<Parameter>();
			send_parameter->add<std::string>("BGM_name", bgmName_);
			send_parameter->add<std::string>("BGM_path", bgmPath_);
			send_parameter->add<bool>("autoFlag", autoPerfectMode_);
			send_parameter->add<size_t>("stage", stageNum_);
			//BGMを停止する
			Sound(bgmName_).stop();
			ON_SCENE_CHANGE(SceneName::PAUSE, send_parameter.get(), StackPopFlag::NON, true);
		}
	}

	//結果画面遷移
	void Game::changeResultScene()
	{
		Sound sound(bgmName_);
		if (sound.getTotalTime() <= sound.getCurrentTime()) 
		{
			if (finish == nullptr)
			{
				finish = ECS::GameEffectsArcheType::CreateFinishEntity(
					"finish", Vec2{ System::SCREEN_WIDTH / 2.f,System::SCREEN_HEIGHT / 2.f }, entityManager_
				);
			}
			else
			{
				if (finish->getComponent<ECS::ExpandComponentSystem>().isEaseEnd())
				{
					fade_->getComponent<ECS::AlphaBlend>().alpha += 10;
				}
			}
			//BGMを停止する
			Sound(bgmName_).stop();
			if (fade_->getComponent<ECS::AlphaBlend>().alpha >= 255)
			{
				auto sendParame = std::make_unique<Parameter>();
				sendParame->add<std::string>("BGM_name", bgmName_);
				sendParame->add<int>("score", scoreNum_);
				sendParame->add<int>("maxcombo", maxComb_);
				sendParame->add<bool>("fullcombo", maxComb_ == msl_.getMaxNoteNum());
				//オートモードは記録しない
				if (!autoPerfectMode_)
				{
					if (!isUpdateScore_)
					{
						switch (stageNum_)
						{
						case 1:
							sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE1, scoreNum_, *entityManager_)
								->getComponent<ECS::ScoreSystem>().isUpdateScore());
							isUpdateScore_ = true;
							break;
						case 2:
							sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE2, scoreNum_, *entityManager_)
								->getComponent<ECS::ScoreSystem>().isUpdateScore());
							isUpdateScore_ = true;
							break;
						case 3:
							sendParame->add<bool>("newrecord", ECS::ScoreArcheType::CreateScoreEntity(ECS::StageHighScore::STAGE3, scoreNum_, *entityManager_)
								->getComponent<ECS::ScoreSystem>().isUpdateScore());
							isUpdateScore_ = true;
							break;
						}
					}

				}
				else
				{
					sendParame->add<bool>("newrecord", false);
				}
				
				ON_SCENE_CHANGE(SceneName::RESULT, sendParame.get(), StackPopFlag::POP, true);
			}
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
	void Game::createRankFont(int rank)
	{
		auto font = ECS::UIArcheType::CreateRankFont("rank", Vec2{ System::SCREEN_WIDTH / 2.f, 200.f }, *entityManager_);
		font->getComponent<ECS::SpriteAnimationDraw>().setIndex(rank);
		entityManager_->refresh();
	}
	const bool Game::isStartUIEnd()
	{
		//CookingStartUI
		//拡大から停止まで
		if (start_ == nullptr)
		{
			//お玉とフライパン
			pan_ = ECS::GameEffectsArcheType::CreateStartUIOrnament(
				"gong", 1, Vec2{ System::SCREEN_WIDTH / 2.f + 30.f,System::SCREEN_HEIGHT / 2.f + 50.f }, 0.f, startUIstopTime, entityManager_
			);
			pan_->addComponent<ECS::SoundGongComponent>(40, -40.f);
			otama_ = ECS::GameEffectsArcheType::CreateStartUIOrnament(
				"gong", 0, Vec2{ System::SCREEN_WIDTH / 2.f - 10.f,System::SCREEN_HEIGHT / 2.f + 60.f }, 0.f, startUIstopTime, entityManager_
			);
			otama_->addComponent<ECS::SoundGongComponent>(40, 40.f);
			//GameStartの文字
			start_ = ECS::GameEffectsArcheType::CreateStartUIFont(
				"start", Vec2{ System::SCREEN_WIDTH / 2.f,System::SCREEN_HEIGHT / 2.f }, startUIstopTime, entityManager_
			);
			//星の座標を算出しておく
			Vec2 starPos[4] = {
				Vec2{ System::SCREEN_WIDTH / 2.f - 200.f,System::SCREEN_HEIGHT / 2.f - 15.f },
				Vec2{ System::SCREEN_WIDTH / 2.f + 200.f,System::SCREEN_HEIGHT / 2.f - 15.f },
				Vec2{ System::SCREEN_WIDTH / 2.f - 150.f,System::SCREEN_HEIGHT / 2.f },
				Vec2{ System::SCREEN_WIDTH / 2.f + 150.f,System::SCREEN_HEIGHT / 2.f }
			};
			//星の角度
			float starAngle[2] = { 30.f, -30.f };
			//星つくる
			for (int i = 0; i < 4; ++i)
			{
				stars_[i] = ECS::GameEffectsArcheType::CreateStartUIOrnament(
					"star", i / 2, starPos[i], starAngle[i % 2], startUIstopTime, entityManager_
				);
			}
		}
		//明示的に更新処理を呼ぶ
		start_->update();
		otama_->update();
		pan_->update();
		if (otama_->getComponent<ECS::Rotation>().val >= 40.f)
		{
			Sound se("nabeHit");
			se.play(false, true);
		}
		for (int i = 0; i < 4; ++i)
		{
			stars_[i]->update();
		}
		//終了する処理
		if (start_->getComponent<ECS::Scale>().val.x >= 10.f)
		{
			//完全に消す
			start_->destroy();
			for (int i = 0; i < 4; ++i)
			{
				stars_[i]->destroy();
			}
			otama_->destroy();
			pan_->destroy();
			return true;
		}
		return false;
	}
}
