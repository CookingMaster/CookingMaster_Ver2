#include "StageSelect.h"
#include "../src/Input/Input.hpp"
#include "../src/Components/ScoreSystem.hpp"
#include "../src/ArcheType/ScoreArcheType.hpp"
#include "../src/Components/CursorMove.hpp"
#include "../src/Class/Sound.hpp"
#include "../src/ArcheType/TestArcheType.hpp"

namespace Scene
{
	
	StageSelect::StageSelect(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager)
		: AbstractScene(sceneTitleChange)
		, entityManager_(entityManager)
		, cnt_(0,10,0,90)
	{
		//セレクト曲
		ResourceManager::GetSound().load("Resource/sound/Welcome.ogg", "selectBGM",SoundType::BGM);
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

		cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
		(
			"menuname",
			Vec2{ 370.f,310.f },
			*entityManager_,
			ENTITY_GROUP::BACK_OBJECT
		))->getComponent<ECS::SpriteAnimationDraw>().setIndex(0);

		cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
		(
			"menuname",
			Vec2{ 370.f,420.f },
			*entityManager_,
			ENTITY_GROUP::BACK_OBJECT
		))->getComponent<ECS::SpriteAnimationDraw>().setIndex(1);

		cursorTargets.emplace_back(ECS::ArcheType::CreateMultiEntity
		(
			"menuname",
			Vec2{ 370.f,530.f },
			*entityManager_,
			ENTITY_GROUP::BACK_OBJECT
		))->getComponent<ECS::SpriteAnimationDraw>().setIndex(2);

		//オプションの文字の位置
		cursorTargets.emplace_back(ECS::ArcheType::CreatePlainEntity
		(
			Vec2{ OPTION_POSITION },
			*entityManager_
		));

		cursor_ = ECS::ArcheType::CreateEntity
		(
			"cursor",
			Vec2{0.f, 0.f },
			*entityManager_,
			ENTITY_GROUP::UI
		);
		cursor_->addComponent<ECS::CursorMove>(cursorTargets);
	}

	void StageSelect::initialize()
	{
		entitySetUp();
		Sound bgm("selectBGM");
		bgm.play(true,false);
	}
	void StageSelect::update()
	{
		
		cursorTargets[3]->getComponent<ECS::Position>().val = OPTION_POSITION + option_->getComponent<ECS::Position>().val;
		if (cursor_->getComponent<ECS::CursorMove>().getIndex() == 3)
		{
			cnt_.add();
			if (!cnt_.isMax())
			{
				backVal_ += 3;
				option_->getComponent<ECS::Position>().val.x += 3;
			}
			
		}
		else
		{
			option_->getComponent<ECS::Position>().val.x -= backVal_;
			backVal_ = 0;
			cnt_.reset();
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
	
	}

}// namespace StageSelect