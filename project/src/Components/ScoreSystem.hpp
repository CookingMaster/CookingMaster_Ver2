﻿/**
* @file ScoreSystem.hpp
* @brief リザルト画面でスコアの演出を行うためのコンポーネント群です
* @author tonarinohito
* @date 2018/10/26
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include <iostream>
#include <fstream>
#include <DxLib.h>

namespace ECS
{
	//!曲ごとにスコアを設定するための定数です
	enum class StageHighScore : size_t
	{
		STAGE1 = 1,		//本来は曲の名前とかにする
		STAGE2,
		STAGE3,
		STAGE4,
	};
	struct ScoreData : public ComponentData
	{
		int val = 0;
		ScoreData() = default;
		ScoreData(const int score) : val(score) {}
	};

	/**
	* @brief  ハイスコアを外部ファイルに保存します
	* @detail コンストラクタでStageHighScoreを入れ保存先を決定します
	* - 
	*/
	class ScoreSystem : public ComponentSystem
	{
	private:
		ScoreData* score_;
		StageHighScore stageName_;
		//得点保存用ファイルを作成
		void wrightScoreData(int score)
		{
			std::ofstream ofs;
			//ファイル名にステージの番号をつける
			ofs.open(std::string("Resource/result/score") + std::to_string(size_t(stageName_)) + ".bin", std::ios::out);

			if (!ofs)
			{
				return;
			}
			ofs << score;
			ofs.close();
		}
		void loadScoreData()
		{
			std::ifstream ifs;
			ifs.open(std::string("Resource/result/score") + std::to_string(size_t(stageName_)) + ".bin", std::ios::in);

			if (!ifs)
			{
				//読み込みに失敗したら新しくファイルを作成する
				wrightScoreData(0);
				return;
			}
			
			int saveScore = 0;
			ifs >> saveScore;
			if (saveScore < score_->val)
			{
				wrightScoreData(score_->val);
			}
			ifs.close();
		}
	public:
		ScoreSystem(const StageHighScore& stageName)
		{
			stageName_ = stageName;
		}
		void initialize() override
		{
			score_ = &entity->getComponent<ScoreData>();
			loadScoreData();
		}
	};

	/**
	* @brief  リザルト用フォントの演出を行います
	* -
	*/
	class ResultEffect : public ComponentSystem
	{
	private:
		Scale* scale_;
		Easing ease_;
	public:
		void initialize() override
		{
			scale_ = &entity->getComponent<Scale>();
			scale_->val = 0;
		}

		void update() override
		{
			ease_.run(Easing::ExpoIn,100.f);
			scale_->val.x = ease_.getVolume(0.f ,3.f - 0.f);
			ease_.run(Easing::ExpoIn, 100.f);
			scale_->val.y = ease_.getVolume(0.f, 3.f - 0.f);
		}
	};
}