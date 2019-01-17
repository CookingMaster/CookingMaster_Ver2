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
		//! 定数
		const int SCORE_GREAT = 80;
		const int SCORE_GOOD = 50;
		//! エンティティマネージャー
		ECS::EntityManager* entityManager_ = nullptr;
		//! フェード
		ECS::Entity* fade_ = nullptr;
		//! 背景
		ECS::Entity* back_ = nullptr;
		//! クロッシュ
		ECS::Entity* cloche_ = nullptr;
		//! 料理
		ECS::Entity* dish_ = nullptr;
		//! 紙吹雪
		std::vector<ECS::Entity*> confetties_{ nullptr };
		//! 評価フォント
		ECS::Entity* evaluation_ = nullptr;
		//! スコア背景
		ECS::Entity* black_ = nullptr;
		//! スコア
		int score_;
		//! 経過フレームカウンタ
		Counter counter_;
		//! ステージ情報
		std::string bgmName_;
		ECS::StageHighScore stage_;
		//! フェードアウトフラグ
		bool isFadeOut_;

		//リザルトの進行度を管理する
		enum Timing {
			CONFETTI = 100,		//紙吹雪出す
			EVALUATION = 140,	//評価フォント出す
			SCORE_BACK = 200,	//スコア用背景出す
			SCOREFONT = 230,	//スコアとコンボの文字出す
			SCORE = 250,		//スコア数字出す
			COMBO = 300,		//コンボ数出す
			FADE_OUT = 550,		//フェードアウト開始
		};

		//! スコアから料理の画像を設定する
		Vec2 setDishImg();
		//! スコアを取得する
		void setStage();
		//! スコアから評価フォントのインデックスを設定する
		int getEvaluationIndex();
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame, ECS::EntityManager* entityManager);
		void initialize() override;
		void update() override;
		void draw() override;
		~Result();
	};

}