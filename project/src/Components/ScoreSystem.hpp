/**
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
	//!曲ごとにスコアを設定
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

	//!スコアを外部ファイルに書き出します
	class ScoreSystem : public ComponentSystem
	{
	private:
		ScoreData* score_;
		StageHighScore stageName_;
		//0が入ったファイルを作成
		void wrightScoreData()
		{
			std::ofstream ofs;
			//ファイル名にステージの番号をつける
			ofs.open(std::string("Resource/result/score") + std::to_string(size_t(stageName_)) + ".bin", std::ios::out);

			if (!ofs)
			{
				return;
			}
			ofs << 0;
			ofs.close();
		}
		void loadScoreData()
		{
			std::ifstream ifs;
			ifs.open(std::string("Resource/result/score") + std::to_string(size_t(stageName_)) + ".bin", std::ios::in);

			if (!ifs)
			{
				//読み込みに失敗したら新しくファイルを作成する
				wrightScoreData();
				return;
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
		}

	};
}