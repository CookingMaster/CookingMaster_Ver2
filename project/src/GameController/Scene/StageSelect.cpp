#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Components/CursorMove.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"
#include "../src/Components/musicName.hpp"
namespace Scene
{
	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneChange)
		, entityManager_(entityManager)
		, cnt_(0,1,0,10)
	{
		//セレクト曲
		ResourceManager::GetSound().load("Resource/sound/BGM/Welcome.ogg", "selectBGM",SoundType::BGM);
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
		//スライダー
		ResourceManager::GetGraph().load("Resource/image/slider.png", "slider");
		ResourceManager::GetGraph().load("Resource/image/slider_bar.png", "bar");
	}

	void StageSelect::entitySetUp()
	{
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
			cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
			(
				"menuname",
				Vec2{ 370.f,310.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);
			cursorTargets.back()->addComponent<ECS::MusicName>("Let'sCooking.wav");

			//1 曲
			cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
			(
				"menuname",
				Vec2{ 370.f,420.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);
			cursorTargets.back()->addComponent<ECS::MusicName>("test.mp3");

			//2 曲
			cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
			(
				"menuname",
				Vec2{ 370.f,530.f },
				*entityManager_,
				ENTITY_GROUP::UI
			))->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);
			cursorTargets.back()->addComponent<ECS::MusicName>("act_bgm.wav");

			//3 オプションの文字の位置
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ OPTION_POSITION },
				*entityManager_
			));

			//4 BGMスライダー 
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ BGM_SLIDER_POSITION },
				*entityManager_
			));
			//5 SEスライダー
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ SE_SLIDER_POSITION },
				*entityManager_
			));
			//6 戻るボタンの位置
			cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
			(
				Vec2{ BACK_POSITION },
				*entityManager_
			));
		}
		//カーソル生成
		{
			cursor_ = ECS::ArcheType::CreateEntity
			(
				"cursor",
				Vec2{ 0.f, 0.f },
				*entityManager_,
				ENTITY_GROUP::UI
			);
			cursor_->addComponent<ECS::CursorMove>(cursorTargets);
		}
		
	}

	void StageSelect::optionSheetMove()
	{
		//オプションに乗ってるやつをオプションエンティティの相対座標に設定する
		{
			cursorTargets[3]->getComponent<ECS::Position>().val = OPTION_POSITION + option_->getComponent<ECS::Position>().val;
			cursorTargets[4]->getComponent<ECS::Position>().val = BGM_SLIDER_POSITION + option_->getComponent<ECS::Position>().val;
			cursorTargets[5]->getComponent<ECS::Position>().val = SE_SLIDER_POSITION + option_->getComponent<ECS::Position>().val;
			cursorTargets[6]->getComponent<ECS::Position>().val = BACK_POSITION + option_->getComponent<ECS::Position>().val;
		}
	
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

		//レイヤー入れ替え
		if (cursor_->getComponent<ECS::CursorMove>().isOptionSelected())
		{
			option_->changeGroup(ENTITY_GROUP::BACK_OBJECT);
		}
		else
		{
			option_->changeGroup(ENTITY_GROUP::BACK);
		}
	}

	void StageSelect::initialize()
	{
		entitySetUp();
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		optionSheetMove();
		auto bgmName = cursor_->getComponent<ECS::CursorMove>().getSelectStage();
		if (bgmName != "")
		{
			auto name = std::make_unique<Parameter>();
			ResourceManager::GetSound().load("Resource/sound/MUSIC/" + bgmName,"stage1", SoundType::BGM);
			name->add<std::string>("BGM_name", "stage1");
			ON_SCENE_CHANGE(SceneName::GAME, name.get(), StackPopFlag::POP, true);
		}
		entityManager_->update();
	}
	void StageSelect::draw()
	{
		entityManager_->orderByDraw(ENTITY_GROUP::MAX);
	}

	StageSelect::~StageSelect()
	{
		entityManager_->allDestory();
		ResourceManager::GetGraph().removeAll();
		ResourceManager::GetSound().remove("selectBGM");
	}

}// namespace StageSelect