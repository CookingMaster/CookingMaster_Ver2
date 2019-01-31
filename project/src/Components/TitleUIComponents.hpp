/**
* @file TitleUIComponents.hpp
* @brief �^�C�g���V�[���Ŏg�p����UI�̂��߂ɍ�����R���|�[�l���g�̕��X
* @author feveleK5563
* @date 2018/11/22
*/

#pragma once
#include "../ECS/ECS.hpp"
#include "Renderer.hpp"
#include "../Utility/Easing.hpp"
#include "../Input/Input.hpp"

namespace ECS
{
	/*!
	@brief �摜��_�ł�����i����������j�A�t���O��true�ɂ���Ə��ł���
	* - AlphaBlend���K�v�A�Ȃ���Ώ���ɍ��
	*/
	class FlashImage final : public ComponentSystem
	{
	private:
		AlphaBlend* alpha_ = nullptr;
		Easing ease_;
		Counter waitCnt_;
		bool bright_;
		bool isDelete_;
		float deleteTime_;

	public:
		FlashImage() :
			waitCnt_(0, 1, 0, 0),
			bright_(false),
			isDelete_(false),
			deleteTime_(0.f) {}

		void initialize() override
		{
			if (!entity->hasComponent<AlphaBlend>())
			{
				entity->addComponent<AlphaBlend>();
			}
			alpha_ = &entity->getComponent<AlphaBlend>();
			alpha_->alpha = 255;
		}

		void update() override
		{
			waitCnt_.add();
			if (!waitCnt_.isMax())
			{
				return;
			}

			if (isDelete_)
			{
				ease_.run(Easing::CubicOut, deleteTime_);
				alpha_->alpha = int(ease_.getVolume(255.f, 0.f));
				if (ease_.isEaseEnd())
				{
					entity->destroy();
				}
			}
			else
			{
				if (bright_)
				{
					ease_.run(Easing::CubicOut, 60.f);
					alpha_->alpha = int(ease_.getVolume(150.f, 255.f));
				}
				else
				{
					ease_.run(Easing::CubicIn, 60.f);
					alpha_->alpha = int(ease_.getVolume(255.f, 150.f));
				}

				if (ease_.isEaseEnd())
				{
					bright_ = !bright_;
					ease_.reset();
				}
			}
		}

		void setIsDelete(bool isDelete, float deleteTime = 30.f)
		{
			isDelete_ = isDelete;
			deleteTime_ = deleteTime;
			ease_.reset();
		}

		void setWaitTime(int waitTime)
		{
			waitCnt_.reset();
			waitCnt_.setCounter(0, 1, 0, waitTime);
		}
	};

	/*!
	@brief �C�[�W���O�ō��W�𑀍삷��
	* - Position���K�v�A�Ȃ���Ώ���ɍ��
	*/
	class EasingPosMove final : public ComponentSystem
	{
	private:
		Position* pos_ = nullptr;		//���W
		Easing ease_;		//�C�[�W���O
		Vec2 start_;		//�X�^�[�g�ʒu
		Vec2 goal_;			//�S�[���ʒu
		float durationTime;	//�o�ߎ���

	public:
		EasingPosMove() :
			start_(0, 0),
			goal_(0, 0),
			durationTime(60.f) {}

		void initialize() override
		{
			if (!entity->hasComponent<Position>())
			{
				entity->addComponent<Position>();
			}
			pos_ = &entity->getComponent<Position>();
		}

		void update() override
		{
			ease_.run(ease_.CircOut, durationTime);
			pos_->val.x = ease_.getVolume(start_.x, goal_.x);
			pos_->val.y = ease_.getVolume(start_.y, goal_.y);
		}

		void setDest(const Vec2& start, const Vec2& goal, float time)
		{
			start_ = start;
			goal_ = goal;
			durationTime = time;
			ease_.reset();
		}

		bool getIsEaseEnd()
		{
			return ease_.isEaseEnd();
		}

	};

	/*!
	@brief �Ȃ񂩂̃{�^�����͂���������A�����Ɏw�肵���֐������s����
	*/
	class AnyInputFunction final : public ComponentSystem
	{
	public:
		typedef void(*Func)(ECS::Entity*, EntityManager&, bool&);

	private:
		Func func_;
		EntityManager& entityManager_;
		bool isPushed;

	public:
		AnyInputFunction(Func func, EntityManager& entityManager) :
			func_(func),
			entityManager_(entityManager),
			isPushed(false){}

		void update() override
		{
			auto& input = Input::Get();
			if (input.getIsAnyInput())
			{
				func_(entity, entityManager_, isPushed);
			}
		}

		bool getIsPushed()
		{
			return isPushed;
		}
	};

	/*!
	@brief �摜�̎w����W�Ɍ������Ċg�傷��
	* - Position��Scale��SpriteDraw���K�v
	*/
	class ZoomIn final : public ComponentSystem
	{
	private:
		Position* pos_ = nullptr;
		Scale* scale_ = nullptr;
		SpriteDraw* sd = nullptr;
		Counter_f cnt_;
		Vec2 zoomPos_;

	public:
		ZoomIn(const float zoomSpd, Vec2 zoomPos) :
			cnt_(1.f, zoomSpd, 1.f, 255.f),
			zoomPos_(zoomPos) {}

		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			pos_->val += zoomPos_;

			scale_ = &entity->getComponent<Scale>();

			sd = &entity->getComponent<SpriteDraw>();
			sd->setPivot(zoomPos_);
		}

		void update() override
		{
			scale_->val.x = cnt_.getCurrentCount();
			scale_->val.y = cnt_.getCurrentCount();
			++cnt_;
		}
	};
}