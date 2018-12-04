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
#include "BasicComponents.hpp"
#include "../ECS/ECS.hpp"

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
		std::array<float, 4> hitJudge_;
		float arrivalBeatTime_;

		NoteState* noteState_;
		AnimatorByFrame* animator_;
		std::array<float, 9> hitTimeLine_;
		Counter transCounter_;
		Counter_f flameCounter_;

	public:
		NoteStateTransition(const std::array<float, 4>& hitJudge, float arrivalBeatTime) :
			hitJudge_(hitJudge),
			arrivalBeatTime_(arrivalBeatTime),
			transCounter_(0, 9) {}

		void initialize() override
		{
			noteState_ = &entity->getComponent<NoteState>();
			animator_ = &entity->getComponent<AnimatorByFrame>();


			//ミリ秒をフレームに変換
			for (auto& it : hitJudge_)
			{
				it = millisecondToFlame(it);
			} 

			/*NON → BAD → GOOD → GREAT → PARFECT → GREAT → GOOD → BAD → NON → MISSED という風に遷移する
			BADの時に入力があるとGRAZEDへ移行する
			GOOD,GREAT,PARFECTの時に入力があるとHITTEDへ移行する
			以下の羅列は各判定開始時間の計算*/
			hitTimeLine_[0] = arrivalBeatTime_ - ((hitJudge_[3] / 2.f) + hitJudge_[2] + hitJudge_[1] + hitJudge_[0]);
			hitTimeLine_[1] = hitTimeLine_[0] + hitJudge_[0];
			hitTimeLine_[2] = hitTimeLine_[1] + hitJudge_[1];
			hitTimeLine_[3] = hitTimeLine_[2] + hitJudge_[2];
			hitTimeLine_[4] = hitTimeLine_[3] + hitJudge_[3];
			hitTimeLine_[5] = hitTimeLine_[4] + hitJudge_[2];
			hitTimeLine_[6] = hitTimeLine_[5] + hitJudge_[1];
			hitTimeLine_[7] = hitTimeLine_[6] + hitJudge_[0];
			hitTimeLine_[8] = hitTimeLine_[7] + 10.f;

			noteState_->val = NoteState::State::NON;
		}

		void update() override
		{
			if (transCounter_.isMax()) return;

			if (flameCounter_.getCurrentCount() > hitTimeLine_[transCounter_.getCurrentCount()])
			{
				transition();
			}
			++flameCounter_;
		}

		/**
		* @brief 入力を受けた後のノーツの状態遷移を行う
		* @return bool 判定が有効だったか否か(有効だったらtrueが返る)
		*/
		bool ActionToChangeNoteState()
		{
			switch (noteState_->val)
			{
			case NoteState::State::NON:
			case NoteState::State::MISSED:
			case NoteState::State::GRAZED:
			case NoteState::State::HITTED:
				return false;

			case NoteState::State::BAD:
				noteState_->val = NoteState::State::GRAZED;
				break;

			case NoteState::State::GOOD:
			case NoteState::State::GREAT:
			case NoteState::State::PARFECT:
				noteState_->val = NoteState::State::HITTED;
				break;
			}
			return true;
		}

		//現在のノーツの状態を取得する
		[[nodiscard]]NoteState::State getNoteState() const
		{
			return noteState_->val;
		}

	private:
		//ミリ秒をフレーム数に変換
		float millisecondToFlame(float ms)
		{
			return (ms / 1000.f) * 60.f;
		}

		//状態を遷移させる
		void transition()
		{
			if (noteState_->val == NoteState::State::MISSED ||
				noteState_->val == NoteState::State::GRAZED ||
				noteState_->val == NoteState::State::HITTED)
				return;

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
			case 8:	noteState_->val = NoteState::State::MISSED;
					animator_->setSpriteNum(0, 1, 2, 2);
					animator_->changeFrame(10);
					animator_->setIsEndStopAnim(true);
					entity->stopComponent<Physics>();
					entity->updateComponent<KillEntity>();
					break;
			default: return;
			}
			++transCounter_;
		}
	};
}