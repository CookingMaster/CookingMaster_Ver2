/**
* @file  BeatByTrigger.hpp
* @brief BPMを利用し、指定拍数ごとにtrueを返す
* @note  リズムに合わせてアニメーションしたりするやつ用
* @author feveleK5563
* @date 2018/12/20
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Class/ResourceManager.hpp"
#include "../System/System.hpp"
#include "Renderer.hpp"
#include "../Utility/Counter.hpp"
#include "../Utility/CalcurationBeat.hpp"
#include "../Class/Sound.hpp"

namespace ECS {

	class BeatByTrigger final : public ComponentSystem
	{
	private:
		int bpm_ = 0;			//BPM
		int beat_ = 0;			//拍子
		float rhythm_ = 0.f;	//リズムを取るタイミング
		float goalTime_ = 0.f;	//次にtrueとなる時間
		Sound sound_;
		bool trigger = false;

	public:
		BeatByTrigger(int bpm, int beat, float rhythm, const std::string& soundName) :
			bpm_(bpm),
			beat_(beat),
			rhythm_(rhythm),
			sound_(soundName) {}

		void initialize() override
		{
			CalcurationBeat beat(bpm_, beat_);
			goalTime_ = beat.calcNote_Millisecond(rhythm_);
		}

		void update() override
		{
			//指定拍毎にトリガーをtrueにする
			if (sound_.getCurrentTime() > goalTime_)
			{
				CalcurationBeat beat(bpm_, beat_);
				goalTime_ += beat.calcNote_Millisecond(rhythm_);
				trigger = true;
			}
			else
			{
				trigger = false;
			}
		}

		//トリガーを取得
		bool getTrigger()
		{
			return trigger;
		}

		//トリガーがtrueになる拍を変更
		void setBeat(float rhythm)
		{
			rhythm_ = rhythm;
		}
	};
}