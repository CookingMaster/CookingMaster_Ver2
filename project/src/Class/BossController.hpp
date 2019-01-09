/**
* @file BossController.hpp
* @brief おやっさん系エンティティをまとめて管理します
* @author tonarinohito
* @date 2018/12/21
*/
#pragma once
#include "../ArcheType/GameEffectsArcheType.hpp"


class BossController final
{
private:
	//顔
	const int GOOD = 1u;
	const int BAD = 0u;

	int currentComb_ = 0;
	int preComb_ = 0;		//前のコンボの値
	bool isSpeek_ = false;	//吹き出しが複製されないようにするためのフラグ
	Counter cnt_;
	ECS::Entity* bossBody_ = nullptr;
	ECS::Entity* bossFace_ = nullptr;
	ECS::Entity* bossBalloon_ = nullptr;
	ECS::Entity* bossMessage_ = nullptr;
	ECS::EntityManager& manager_;
	void setComb(const int& num)
	{
		bossBalloon_ = ECS::GameEffectsArcheType::CreateBossBalloon("balloon", Vec2{ 300,100 }, 120, &manager_);
		bossBalloon_->getComponent<ECS::Scale>().val = 2;
		bossMessage_ = ECS::GameEffectsArcheType::CreateBossMessage("number", Vec2{ 200,100 }, ECS::Rectangle(0, 0, 50, 100), num, 120, &manager_);
	}
public:
	BossController(ECS::EntityManager& manager) :
		cnt_(120,1,0,120),
		manager_(manager)
	{
		ResourceManager::GetGraph().loadDiv("Resource/image/chef.png","Boss",3,3,1,132,243);
		ResourceManager::GetGraph().loadDiv("Resource/image/chef_face.png", "BossFace", 2,2, 1, 86, 70);
		ResourceManager::GetGraph().load("Resource/image/number2.png", "number");//かり
		ResourceManager::GetGraph().load("Resource/image/pause_button.png", "balloon");//かり
		bossBody_ = ECS::GameEffectsArcheType::CreateBossBody("Boss", Vec2{100.f,235.f}, &manager_);
		bossFace_ = ECS::GameEffectsArcheType::CreateBossFace("BossFace", Vec2{ 10.f,12.f }, &manager_);
		bossBody_->getComponent<ECS::Canvas>().addChild(bossFace_);
		bossFace_->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
	}
	void speekComb(const int& setCombNum)
	{
		currentComb_ = setCombNum;
		//120f後に元の表情に戻す
		if (cnt_.isMin())
		{
			bossFace_->getComponent<ECS::SpriteAnimationDraw>().drawDisable();
		}
		else
		{
			cnt_.sub();
		}

		//コンボ継続中にミスをした
		if (preComb_ != 0 && currentComb_ == 0)
		{
			bossFace_->getComponent<ECS::SpriteAnimationDraw>().drawEnable();
			bossFace_->getComponent<ECS::SpriteAnimationDraw>().setIndex(BAD);
			cnt_.setCounter(120, 1, 0, 120);
		}
		//とりあえず10の倍数で吹き出しを出す
		if (setCombNum % 10 == 0 && !isSpeek_ && setCombNum != 0)
		{
			
			isSpeek_ = true;
			cnt_.setCounter(120, 1, 0, 120);
			bossFace_->getComponent<ECS::SpriteAnimationDraw>().drawEnable();
			bossFace_->getComponent<ECS::SpriteAnimationDraw>().setIndex(GOOD);
			setComb(setCombNum);
		}
		else if(setCombNum % 10 != 0 && isSpeek_)
		{
			isSpeek_ = false;
		}
		preComb_ = currentComb_;
	}
	
};