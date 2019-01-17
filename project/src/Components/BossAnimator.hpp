/**
* @file BossAnimator.hpp
* @brief �������������Y���ɍ��킹�ăA�j���[�V����������R���|�[�l���g
* @author feveleK5563
* @date 2019.1.10
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "Animator.hpp"
#include "BeatByTrigger.hpp"

namespace ECS
{
	class BossAnimator final : public ComponentSystem
	{
	private:
		Animator* animator_ = nullptr;
		SpriteAnimationDraw* sad_ = nullptr;
		BeatByTrigger* beatTrigger_ = nullptr;
		Position* position_ = nullptr;
		bool shoulderUp = false;
		Vec2 pos_ =  Vec2(0, 0);

	public:
		void initialize() override
		{
			if (!entity->hasComponent<Animator>())
			{
				entity->addComponent<Animator>();
			}
			animator_ = &entity->getComponent<Animator>();
			animator_->setSpriteNum(0, 0, 3, 1, true);
			animator_->setIsEndStopAnim(true);
			animator_->changeFrame(2);

			sad_ = &entity->getComponent<SpriteAnimationDraw>();
			beatTrigger_ = &entity->getComponent<BeatByTrigger>();
			position_ = &entity->getComponent<Position>();
			pos_ = position_->val;

			//���グ��Ԃ���X�^�[�g
			sad_->setIndex(0);
			animator_->setIsMinusAnim(true);
			shoulderUp = true;
		}

		void update() override
		{
			//���グ�̓������s���Ă��鎞
			if (shoulderUp)
			{
				//�������S�ɏオ�����牺����
				if (animator_->isAnimEnd())
				{
					animator_->setIsMinusAnim(false);
					shoulderUp = false;
				}
			}
			//�������̓������s���Ă��鎞
			else
			{
				//�^�C�~���O�ǂ������グ��
				if (beatTrigger_->getTrigger())
				{
					animator_->setIsMinusAnim(true);
					shoulderUp = true;
				}
			}

			//���݂̃C���f�b�N�X�l�ɂ���Ă��������̍�����ς���
			switch (sad_->getIndex())
			{
			case 0: position_->val.y = pos_.y + 6; break;

			case 1: position_->val.y = pos_.y + 3; break;

			case 2: position_->val.y = pos_.y; break;
			}
		}
	};
}