#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Components/CursorMove.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/ArcheType.hpp"
#include "../src/Components/musicName.hpp"

namespace Scene
{
	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneChange)
		, entityManager_(entityManager)
		, cnt_(0,1,0,10)
	{
		//Fade
		ResourceManager::GetGraph().load("Resource/image/fade_black.png", "fade");
		//セレクト曲
		ResourceManager::GetSound().load("Resource/sound/BGM/Welcome.ogg", "selectBGM",SoundType::BGM);
		//オプション決定音
		ResourceManager::GetSound().load("Resource/sound/SE/turnthepaper.wav", "turn", SoundType::SE);
		//セレクト効果音
		ResourceManager::GetSound().load("Resource/sound/SE/select.wav", "selectSE", SoundType::SE);
		//音量調整効果音
		ResourceManager::GetSound().load("Resource/sound/SE/volume.ogg", "volume", SoundType::SE);
		//カーソル移動
		ResourceManager::GetSound().load("Resource/sound/SE/movecursor.ogg", "move", SoundType::SE);
		//背景
		ResourceManager::GetGraph().load("Resource/image/menuback.png", "back"); 
		//本
		ResourceManager::GetGraph().load("Resource/image/menu2.png", "menu");
		//文字
		ResourceManager::GetGraph().loadDiv("Resource/image/menuname.png", "menuname",3,1,3,240,80);
		//オプション
		ResourceManager::GetGraph().load("Resource/image/option2.png", "option");
		//カーソル
		ResourceManager::GetGraph().load("Resource/image/menu_cursor.png", "cursor");
		//キラキラ
		ResourceManager::GetGraph().loadDiv("Resource/image/effect_menu.png", "effect",3,3,1,128,128);
		//スライダー
		ResourceManager::GetGraph().load("Resource/image/sliderempty.png", "slider");
		ResourceManager::GetGraph().load("Resource/image/sliderfull.png", "slider_full");
		ResourceManager::GetGraph().load("Resource/image/sliderbutton.png", "bar");
		//難易度の星
		ResourceManager::GetGraph().loadDiv("Resource/image/star.png", "star", 2,2,1,55,53);
		//フォント
		ResourceManager::GetGraph().load("Resource/image/number2.png", "number");
		//料理
		ResourceManager::GetGraph().loadDiv("Resource/image/dish_menu.png", "dish",3,3,1,256,256);
		//サウンド情報の読み込み
		std::ifstream ifs("Resource/system/gain.bin");
		ifs >> bgmVal_ >> seVal_;
	}

	void StageSelect::entitySetUp()
	{
		const Vec2 OPTION_POSITION{ 400.f,55.f };
		const Vec2 BGM_FONT_POSITION{ 110.f, 150.f };
		const Vec2 SE_FONT_POSITION{ 110.f, 330.f };
		const Vec2 BGM_SLIDER_POSITION{ 50.f, 195.f };
		const Vec2 SE_SLIDER_POSITION{ 50.f, 375.f };
		const Vec2 BACK_POSITION{ 115.f, 522.f };
		const Vec2 DISH_POSITION{ 680.f, 140.f };
		ECS::ArcheType::CreateEntity
		(
			"back",
			Vec2{ 0.f,0.f },
			*entityManager_,
			ENTITY_GROUP::BACK
		);

		option_ = ECS::ArcheType::CreateEntity
		(
			"option",
			Vec2{ 670.f,50.f },
			*entityManager_,
			ENTITY_GROUP::BACK
		);
		option_->addComponent<ECS::Canvas>();

		ECS::ArcheType::CreateEntity
		(
			"menu",
			Vec2{ 245.f,55.f },
			*entityManager_,
			ENTITY_GROUP::BACK_OBJECT
		);

		//ターゲット(アイコンが指すエンティティ)
		{
			//0 曲
			cursorTargets.emplace_back(ECS::ArcheType::CreateAnimationEntity
			(
				"menuname",
				Vec2{ 330.f,310.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			cursorTargets.back()->addComponent<ECS::MusicName>("Cooking_antipasto.ogg");

			//1 曲
			cursorTargets.emplace_back(ECS::ArcheType::CreateAnimationEntity
			(
				"menuname",
				Vec2{ 330.f,420.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			cursorTargets.back()->addComponent<ECS::MusicName>("Coocking_galantine.wav");

			//2 曲
			cursorTargets.emplace_back(ECS::ArcheType::CreateAnimationEntity
			(
				"menuname",
				Vec2{ 330.f,530.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);
			cursorTargets.back()->addComponent<ECS::MusicName>("Cooking_cuisine.wav");

			//3 オプションの文字の位置
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ OPTION_POSITION },
				*entityManager_
			));

			//4 BGM
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ BGM_FONT_POSITION },
				*entityManager_
			));
			//5 SE
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ SE_FONT_POSITION },
				*entityManager_
			));
			//6 戻るボタンの位置
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ BACK_POSITION },
				*entityManager_
			));
		}

		//スライダー生成
		{
			bgmSlider_ = ECS::ArcheType::CreateEntity
			(
				"slider",
				Vec2{ BGM_SLIDER_POSITION },
				*entityManager_,
				ENTITY_GROUP::BACK
			);
			seSlider_ = ECS::ArcheType::CreateEntity
			(
				"slider",
				Vec2{ SE_SLIDER_POSITION },
				*entityManager_,
				ENTITY_GROUP::BACK
			);
			bgmFullSlider_ = ECS::ArcheType::CreateRectEntity
			(
				"slider_full",
				Vec2{ BGM_SLIDER_POSITION },
				ECS::Rectangle(0,0,0,80),
				*entityManager_,
				ENTITY_GROUP::BACK
			);
			seFullSlider_ = ECS::ArcheType::CreateRectEntity
			(
				"slider_full",
				Vec2{ SE_SLIDER_POSITION },
				ECS::Rectangle(0, 0, 0, 80),
				*entityManager_,
				ENTITY_GROUP::BACK
			);
			bgmBar_ = ECS::ArcheType::CreateEntity
			(
				"bar",
				Vec2{ 0.f,0.f },
				*entityManager_,
				ENTITY_GROUP::LAYER1
			);
			seBar_ = ECS::ArcheType::CreateEntity
			(
				"bar",
				Vec2{ 0.f,0.f },
				*entityManager_,
				ENTITY_GROUP::LAYER1
			);
		}
		//カーソル生成
		{
			cursor_ = ECS::ArcheType::CreateEntity
			(
				"cursor",
				Vec2{ -100.f, -100.f },
				*entityManager_,
				ENTITY_GROUP::UI
			);
			cursor_->addComponent<ECS::CursorMove>(cursorTargets);
		}

		//星生成
		{
			float i = 0;
			for (auto& it : star_)
			{
				it = ECS::ArcheType::CreateAnimationEntity
				(
					"star",
					Vec2{ 740.f + 55.f * i, 420.f },
					*entityManager_,
					ENTITY_GROUP::BACK_OBJECT
				);
				it->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
				++i;
			}	
		}
		//キラキラ
		{
			class CyclicalAlpha final : public ECS::ComponentSystem
			{
			private:
				ECS::AlphaBlend* a_ = nullptr;
				int add_ = 5;
				int initAlpha_ = 0;
			public:
				CyclicalAlpha(const int setAlpha, const int addSpeed)
				{
					initAlpha_ = setAlpha;
					add_ = addSpeed;
				}
				void initialize() override
				{
					a_ = &entity->getComponent<ECS::AlphaBlend>();
					a_->alpha = initAlpha_;
				}
				void update() override
				{
					if (a_->alpha >= 255 || a_->alpha <= 0)
					{
						add_ *= -1;
					}
					a_->alpha += add_;
				}
			};

			effects_[0] = ECS::ArcheType::CreateAnimationEntity
			(
				"effect",
				Vec2{ DISH_POSITION.x + 40 , DISH_POSITION.y },
				*entityManager_,
				ENTITY_GROUP::EFFECT
			);
			effects_[0]->addComponent<CyclicalAlpha>(222,4);
			effects_[0]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			effects_[1] = ECS::ArcheType::CreateAnimationEntity
			(
				"effect",
				Vec2{ DISH_POSITION.x -30 ,DISH_POSITION.y + 30 },
				*entityManager_,
				ENTITY_GROUP::EFFECT
			);
			effects_[1]->addComponent<CyclicalAlpha>(120,6);
			effects_[1]->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			effects_[2] = ECS::ArcheType::CreateAnimationEntity
			(
				"effect",
				Vec2{ DISH_POSITION.x , DISH_POSITION.y - 20 },
				*entityManager_,
				ENTITY_GROUP::EFFECT
			);
			effects_[2]->addComponent<CyclicalAlpha>(90,8);
			effects_[2]->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);
			effects_[3] = ECS::ArcheType::CreateAnimationEntity
			(
				"effect",
				Vec2{ DISH_POSITION.x + 160, DISH_POSITION.y + 140 },
				*entityManager_,
				ENTITY_GROUP::EFFECT
			);
			effects_[3]->addComponent<CyclicalAlpha>(190, 3);
			effects_[3]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
		}
		//料理
		{
			dish_[0] = ECS::ArcheType::CreateAnimationEntity
			(
				"dish",
				Vec2{ DISH_POSITION },
				*entityManager_,
				ENTITY_GROUP::BACK_OBJECT
			);
			dish_[0]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			
			dish_[1] = ECS::ArcheType::CreateAnimationEntity
			(
				"dish",
				Vec2{ DISH_POSITION },
				*entityManager_,
				ENTITY_GROUP::BACK_OBJECT
			);
			dish_[1]->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			dish_[2] = ECS::ArcheType::CreateAnimationEntity
			(
				"dish",
				Vec2{ DISH_POSITION },
				*entityManager_,
				ENTITY_GROUP::BACK_OBJECT
			);
			dish_[2]->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);
		}
		//数字
		{
			score_ = ECS::ScoreArcheType::CreateSelectScoreEntity
			(
				"number",
				Vec2{ 740.f, 620.f },
				ECS::StageHighScore::STAGE1,
				*entityManager_
			);
				
		}
		fade_ = ECS::ArcheType::CreateEntity
		(
			"fade",
			Vec2{ 0.f,0.f },
			*entityManager_,
			ENTITY_GROUP::TOP_FADE
		);
		fade_->getComponent<ECS::SpriteDraw>().drawDisable();
		fade_->getComponent<ECS::AlphaBlend>().alpha = 0;

		//オプションに乗ってるやつをオプションエンティの相対座標に設定する
		option_->getComponent<ECS::Canvas>().addChild(cursorTargets[3]);
		option_->getComponent<ECS::Canvas>().addChild(cursorTargets[4]);
		option_->getComponent<ECS::Canvas>().addChild(cursorTargets[5]);
		option_->getComponent<ECS::Canvas>().addChild(cursorTargets[6]);
		option_->getComponent<ECS::Canvas>().addChild(bgmSlider_);
		option_->getComponent<ECS::Canvas>().addChild(bgmFullSlider_);
		option_->getComponent<ECS::Canvas>().addChild(seSlider_);
		option_->getComponent<ECS::Canvas>().addChild(seFullSlider_);

	}

	void StageSelect::optionSheetMove()
	{
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 3)
		{
			cnt_.add();
			if (!cnt_.isMax())
			{
				constexpr int ADD_SPEED = 4;
				backVal_ += ADD_SPEED;
				option_->getComponent<ECS::Position>().val.x += ADD_SPEED;
			}
		}
		else
		{
			//進めた分を戻す
			option_->getComponent<ECS::Position>().val.x -= backVal_;
			backVal_ = 0;
			cnt_.reset();
		}
	}

	void StageSelect::setSoundVolume()
	{
		auto& cursorMov = cursor_->getComponent<ECS::CursorMove>();
		if (cursorMov.isSiliderSelect())
		{
			//BGM
			if (cursorMov.getIndex() == 4u)
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) >= 1 && bgmVal_ <= MasterSound::MAX_GAIN)
				{
					bgmVal_ += 0.01f;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) >= 1 && bgmVal_ >= MasterSound::MIN_GAIN)
				{
					bgmVal_ -= 0.01f;
				}
			}
			//SE
			if (cursorMov.getIndex() == 5u)
			{
				if (Input::Get().getKeyFrame(KEY_INPUT_RIGHT) >= 1 && seVal_ <= MasterSound::MAX_GAIN)
				{
					seVal_ += 0.01f;
				}
				if (Input::Get().getKeyFrame(KEY_INPUT_LEFT) >= 1 && seVal_ >= MasterSound::MIN_GAIN)
				{
					seVal_ -= 0.01f;
				}
			}
		}
		
		//バーの位置をセット
		constexpr float MAX_GAUGE_SIZE = 250;
		bgmBar_->getComponent<ECS::Position>().val.x = bgmSlider_->getComponent<ECS::Position>().val.x + (MAX_GAUGE_SIZE * bgmVal_);
		seBar_->getComponent<ECS::Position>().val.x = seSlider_->getComponent<ECS::Position>().val.x + (MAX_GAUGE_SIZE * seVal_);
		bgmBar_->getComponent<ECS::Position>().val.y = bgmSlider_->getComponent<ECS::Position>().val.y;
		seBar_->getComponent<ECS::Position>().val.y = seSlider_->getComponent<ECS::Position>().val.y;
		//赤いゲージ増減
		constexpr int ADJUST = 20;	//座標をいい感じにする補正値
		bgmFullSlider_->getComponent<ECS::Rectangle>().w = static_cast<int>(bgmBar_->getComponent<ECS::Position>().val.x -
			option_->getComponent<ECS::Position>().val.x - ADJUST);
		seFullSlider_->getComponent<ECS::Rectangle>().w = static_cast<int>(seBar_->getComponent<ECS::Position>().val.x -
			option_->getComponent<ECS::Position>().val.x - ADJUST);
		//音量更新
		MasterSound::Get().setAllBGMGain(bgmVal_);
		MasterSound::Get().setAllSEGain(seVal_);
	}

	void StageSelect::changeLayer()
	{
		//レイヤー入れ替え
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 3u
			&& cursor_->getComponent<ECS::CursorMove>().isOptionSelected()
			&& Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			option_->changeGroup(ENTITY_GROUP::BACK_OBJECT);
			bgmSlider_->changeGroup(ENTITY_GROUP::UI);
			seSlider_->changeGroup(ENTITY_GROUP::UI);
			bgmFullSlider_->changeGroup(ENTITY_GROUP::UI);
			seFullSlider_->changeGroup(ENTITY_GROUP::UI);
			bgmBar_->changeGroup(ENTITY_GROUP::UI);
			seBar_->changeGroup(ENTITY_GROUP::UI);
			score_->changeGroup(ENTITY_GROUP::LAYER1);
			effects_[0]->changeGroup(ENTITY_GROUP::BACK);
			effects_[1]->changeGroup(ENTITY_GROUP::BACK);
			effects_[2]->changeGroup(ENTITY_GROUP::BACK);
			effects_[3]->changeGroup(ENTITY_GROUP::BACK);
		}
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 3u
			&& !cursor_->getComponent<ECS::CursorMove>().isOptionSelected()
			&& Input::Get().getKeyFrame(KEY_INPUT_Z) == 1)
		{
			option_->changeGroup(ENTITY_GROUP::BACK);
			bgmSlider_->changeGroup(ENTITY_GROUP::BACK);
			seSlider_->changeGroup(ENTITY_GROUP::BACK);
			bgmFullSlider_->changeGroup(ENTITY_GROUP::BACK);
			seFullSlider_->changeGroup(ENTITY_GROUP::BACK);
			bgmBar_->changeGroup(ENTITY_GROUP::LAYER1);
			seBar_->changeGroup(ENTITY_GROUP::LAYER1);
			score_->changeGroup(ENTITY_GROUP::UI);
			effects_[0]->changeGroup(ENTITY_GROUP::EFFECT);
			effects_[1]->changeGroup(ENTITY_GROUP::EFFECT);
			effects_[2]->changeGroup(ENTITY_GROUP::EFFECT);
			effects_[3]->changeGroup(ENTITY_GROUP::EFFECT);
		}
	}

	void StageSelect::showDishParameter()
	{
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 0u)
		{
			star_[0]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			star_[1]->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			star_[2]->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			dish_[0]->getComponent<ECS::SpriteAnimationDraw>().drawEnable();
			dish_[1]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			dish_[2]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			int score = score_->getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE1);
			score_->getComponent<ECS::DrawFont2>().setNumber(score);
		}
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 1u)
		{
			star_[0]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			star_[1]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			star_[2]->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			dish_[0]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			dish_[1]->getComponent<ECS::SpriteAnimationDraw>().drawEnable();
			dish_[2]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			int score = score_->getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE2);
			score_->getComponent<ECS::DrawFont2>().setNumber(score);
		}
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 2u)
		{
			star_[0]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			star_[1]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			star_[2]->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			dish_[0]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			dish_[1]->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
			dish_[2]->getComponent<ECS::SpriteAnimationDraw>().drawEnable();
			int score = score_->getComponent<ECS::ScoreSystem>().getHighScore(ECS::StageHighScore::STAGE3);
			score_->getComponent<ECS::DrawFont2>().setNumber(score);
		}
	}

	void StageSelect::initialize()
	{
		entitySetUp();
		for (auto& it : dish_)
		{
			it->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
		}
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		entityManager_->update();
		showDishParameter();
		optionSheetMove();
		changeLayer();
		setSoundVolume();
		const auto bgm_path = cursor_->getComponent<ECS::CursorMove>().getSelectStage();
		if (bgm_path != "")
		{
			if (!isPlay_)
			{
				stageNum_ = cursor_->getComponent<ECS::CursorMove>().getStageNumber();
				std::ofstream ofs("Resource/system/gain.bin");
				ofs << bgmVal_ << "\n" << seVal_;
				isPlay_ = true;
				bgmPath_ = bgm_path;
				fade_->getComponent<ECS::SpriteDraw>().drawEnable();
				Sound se("selectSE");
				se.play(false,true);
			}
		}
		if (isPlay_)
		{
			cursor_->stopComponent<ECS::CursorMove>();
			fade_->getComponent<ECS::AlphaBlend>().alpha += 6;
		}


		if (fade_->getComponent<ECS::AlphaBlend>().alpha >= 255)
		{
			auto parameter = std::make_unique<Parameter>();
			parameter->add<std::string>("BGM_name", "stage" + std::to_string(stageNum_));
			parameter->add<size_t>("stageNum", stageNum_);
			parameter->add<std::string>("BGM_path", bgmPath_);
			ON_SCENE_CHANGE(SceneName::GAME, parameter.get(), StackPopFlag::POP, true);
		}
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	}

	StageSelect::~StageSelect()
	{
		entityManager_->removeAll();
		ResourceManager::GetGraph().removeAll();
		ResourceManager::GetSound().remove("selectBGM");
	}

}// namespace StageSelect