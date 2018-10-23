/**
* @file  Animator.hpp
* @brief �A�j���[�V�����֘A�̃R���|�[�l���g�ł�
* @note  BPM�A�܂��̓t���[���ɉ������A�j���[�V����������
* @author yokota0717
* @date 2018/10/16
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Class/ResourceManager.hpp"
#include "../System/System.hpp"
#include "Renderer.hpp"
#include "../Utility/Counter.hpp"

namespace ECS {
	/*!
	@brief BPM�ɉ����ăA�j���[�V��������`��@�\�ł��B�摜�̒��S����ł�
	* - SpriteAnimationDraw���K�v�ł��B
	*/
	class AnimatorByBPM final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw* animation_ = nullptr;
		float bpm_ = 0;
		DWORD beat_ = 0;
		DWORD start_ = 0;
		std::string soundname_ = "";
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!�`�悷��摜��X�����̃C���f�b�N�X�̍ŏ�
		int ymin_ = 0;		//!�`�悷��摜��Y�����̃C���f�b�N�X�̍ŏ�
		int maxXnum_ = 0;	//!�`�悷��摜��X�����̖���
		int maxYnum_ = 0;	//!�`�悷��摜��Y�����̖���
	public:
		//!�Ȗ���BPM���w�肵�ď��������܂�
		AnimatorByBPM(const char* soundname, const int bpm)
		{
			soundname_ = soundname;
			setBPM(static_cast<float>(bpm));
			calcBeat();
		}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			start_ = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
		}
		void update() override
		{
			DWORD now = GetSoundCurrentTime(ResourceManager::GetSound().getHandle(soundname_));
			if (now - start_ >= beat_)
			{
				++indexX_;
				if (indexX_ > maxXnum_)
				{
					indexX_ = xmin_;
					if (maxYnum_ > 1)
					{
						++indexY_;
						if (indexY_ > maxYnum_)
						{
							indexY_ = ymin_;
						}
					}
				}
				start_ = now;
			}
			animation_->setIndex(indexX_ * (indexY_ + 1));
		}
		//!BPM��ݒ肷��
		void setBPM(const float bpm)
		{
			bpm_ = bpm;
		}
		/**
		* @brief 1���̒���(ms)���v�Z����
		* @note 1000(ms) * 60(sec/min) / bpm(beat/min)
		*/
		void calcBeat()
		{
			beat_ = static_cast<DWORD>(1000 * (60.f / bpm_));
		}
		/**
		* @brief �A�j���[�V�����̃C���f�b�N�X����ݒ肷��
		* @param minX X�̃C���f�b�N�X�̍ŏ�
		* @param minY Y�̃C���f�b�N�X�̍ŏ�
		* @param maxX X�̖���
		* @param maxY Y�̖���
		* @note �g�p��FY��0�ԖځAX��1~5�ԖڂŃA�j���[�V��������
		* -
		*/
		void setSpriteNum(const int xmin, const int ymin, const int xnum, const int ynum)
		{
			xmin_ = xmin;
			ymin_ = ymin;
			indexX_ = xmin;
			indexY_ = ymin;
			maxXnum_ = xnum;
			maxYnum_ = ynum;
		}
	};

	/*!
	@brief �w��t���[�����ɃA�j���[�V��������`��@�\�ł��B�摜�̒��S����ł�
	* - SpriteAnimationDraw���K�v�ł��B
	*/
	class AnimatorByFrame final : public ComponentSystem
	{
	private:
		SpriteAnimationDraw * animation_ = nullptr;
		Counter counter_;
		int frame_ = 0;		//!�A�j���[�V��������t���[����
		int indexX_ = 0;
		int indexY_ = 0;
		int xmin_ = 0;		//!�`�悷��摜��X�����̃C���f�b�N�X�̍ŏ�
		int ymin_ = 0;		//!�`�悷��摜��Y�����̃C���f�b�N�X�̍ŏ�
		int maxXnum_ = 0;	//!�`�悷��摜��X�����̖���
		int maxYnum_ = 0;	//!�`�悷��摜��Y�����̖���
	public:
		//!�Ȗ���BPM���w�肵�ď��������܂�
		AnimatorByFrame(const int frame)
			:
			frame_(frame)
		{}
		void initialize() override
		{
			animation_ = &entity->getComponent<SpriteAnimationDraw>();
			counter_.SetCounter(0, 1, 0, frame_);
		}
		void update() override
		{
			if (++counter_ >= frame_)
			{
				++indexX_;
				if (indexX_ > maxXnum_)		//
				{
					indexX_ = xmin_;
					if (maxYnum_ > 1)
					{
						++indexY_;
						if (indexY_ > maxYnum_)
						{
							indexY_ = ymin_;
						}
					}
				}
			}
			animation_->setIndex(indexX_ * (indexY_ + 1));
		}
		/**
		* @brief �A�j���[�V�����̃C���f�b�N�X����ݒ肷��
		* @param minX X�̃C���f�b�N�X�̍ŏ�
		* @param minY Y�̃C���f�b�N�X�̍ŏ�
		* @param maxX X�̖���
		* @param maxY Y�̖���
		* @note �g�p��FY��0�ԖځAX��1~5�ԖڂŃA�j���[�V��������
		* -
		*/
		void setSpriteNum(const int xmin, const int ymin, const int xnum, const int ynum)
		{
			xmin_ = xmin;
			ymin_ = ymin;
			indexX_ = xmin;
			indexY_ = ymin;
			maxXnum_ = xnum;
			maxYnum_ = ynum;
		}
	};
}