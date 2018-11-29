/**
* @file Result.h
* @brief リザルトシーン
* @author yokota0717
* @date 2018.11.22
* @param history
#- 2018.11.27 yokota0717
- クロッシュと料理追加
- 紙吹雪追加
#- 2018.11.29 yokota0717
- スコア読み込み暫定版
- 現時点ではハイスコアを読み込み、それをもとに料理の画像を変えている
*/

#pragma once
#include "../../ECS/ECS.hpp"
#include "Parameter.hpp"
#include "../Scene/SceneManager.hpp"
#include "../../Utility/Counter.hpp"
#include "../../ArcheType/ScoreArcheType.hpp"


namespace Scene
{
	class Result : public AbstractScene
	{
	private:
		ECS::EntityManager* entityManager_;
		//! 背景
		ECS::Entity* back_;
		//! クロッシュ
		ECS::Entity* cloche_;
		//! 料理
		ECS::Entity* dish_;
		//! 紙吹雪
		std::vector<ECS::Entity*> confetties_;
		//! スコア
		int score_;
		//! 経過フレームカウンタ
		Counter counter_;
		//! ステージ情報
		std::string bgmName_;

		//! スコアから料理の画像を設定する
		float setDish();
		//! スコアを取得する
		void setScore();
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}