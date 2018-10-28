/**
* @file NoteStateTransition.hpp
* @brief ノーツの状態遷移を行う
* @author feveleK5563
* @date 2018/10/26
*/

#pragma once
#include <array>
#include "../ECS/ECS.hpp"
#include "Animator.hpp"
#include "../Utility/Counter.hpp"

namespace ECS
{
	struct NoteState : public ComponentData
	{
		enum struct State : short
		{
			NON,		//判定なし
			BAD,		//かする
			GOOD,		//当たる(凡)
			GREAT,		//当たる(良)
			PARFECT,	//当たる(優)

			MISSED,		//当たらなかった
			GRAZED,		//かすった
			HITTED,		//当たった
		} val;

		NoteState(): val(State::NON){}
	};

	/**
	* @brief ノーツの状態遷移管理マン
	* - NoteState、AnimatorByFrameが必要
	*/
	class NoteStateTransition : public ComponentSystem
	{
	private:
		NoteState* noteState_;
		AnimatorByFrame* animator_;
		std::array<float, 9> hitTimeLine_;
		Counter transCounter_;
		Counter_f flameCounter_;

	public:
		NoteStateTransition(std::array<float, 4> hitJudge, float arrivalBeatTime)
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<AnimatorByFrame>();

			/*
			NON → BAD → GOOD → GREAT → PARFECT → GREAT → GOOD → BAD → NON → MISSED という風に遷移する
			BADの時に入力があるとGRAZEDへ移行する
			GOOD,GREAT,PARFECTの時に入力があるとHITTEDへ移行する
			以下の羅列は各判定開始時間の計算
			*/
			hitTimeLine_[0] = arrivalBeatTime - (hitJudge[3] / 2.f) - hitJudge[2] - hitJudge[1] - hitJudge[0];
			hitTimeLine_[1] = hitTimeLine_[0] + hitJudge[0];
			hitTimeLine_[2] = hitTimeLine_[1] + hitJudge[1];
			hitTimeLine_[3] = hitTimeLine_[2] + hitJudge[2];
			hitTimeLine_[4] = hitTimeLine_[3] + hitJudge[3];
			hitTimeLine_[5] = hitTimeLine_[4] + hitJudge[2];
			hitTimeLine_[6] = hitTimeLine_[5] + hitJudge[1];
			hitTimeLine_[7] = hitTimeLine_[6] + hitJudge[0];
			hitTimeLine_[8] = arrivalBeatTime * 2.f;

			//ミリ秒をフレームに変換
			for (auto& it : hitTimeLine_)
			{
				it = millisecondToFlame(it);
			}

			noteState_->val = NoteState::State::NON;
		}

		void update() override
		{
			if (int(flameCounter_.getCurrentCount()) > hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transition();
			}
			++flameCounter_;
		}

		NoteState::State getNoteState()
		{
			return noteState_->val;
		}

	private:
		//ミリ秒をフレーム数に変換
		float millisecondToFlame(float ms)
		{
			return ms * (60.f / 1000.f);
		}

		//状態を遷移させる
		void transition()
		{
			switch (transCounter_.getCurrentCount())
			{
			case 0:	noteState_->val = NoteState::State::BAD;		break;
			case 1:	noteState_->val = NoteState::State::GOOD;		break;
			case 2:	noteState_->val = NoteState::State::GREAT;		break;
			case 3:	noteState_->val = NoteState::State::PARFECT;	break;
			case 4:	noteState_->val = NoteState::State::GREAT;		break;
			case 5:	noteState_->val = NoteState::State::GOOD;		break;
			case 6:	noteState_->val = NoteState::State::BAD;		break;
			case 7:	noteState_->val = NoteState::State::NON;		break;
			case 8:	noteState_->val = NoteState::State::MISSED;		break;
			default: return;
			}
			++transCounter_;
		}
	};
}