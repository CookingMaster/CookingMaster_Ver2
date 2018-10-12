/**
* @file NotesCreator.hpp
* @brief �f�[�^���󂯎��A�������Ƀm�[�c�𐶐�����
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "NotesAndScoreData.hpp"
#include "../Utility/Counter.hpp"
#include "../Utility/CalcurationBeat.hpp"

class NotesCreator
{
private:
	int bpm_;
	int offsetTime_;
	TCounter<int> counter_;

public:
	//BPM, OffsetTime�����Z�b�g����
	void resetData(int bpm, int offsetTime)
	{
		bpm_ = bpm;
		offsetTime_ = offsetTime;
		counter_.reset();
	}

	//�X�V����
	void run(const std::vector<NotesData>& notesData, const ScoreData& scoreData)
	{
		CalcurationBeat beat((float)bpm_);

		++counter_;
		//�ꏬ�ߖ��Ƀm�[�c�𐶐�����
		if (counter_.getCurrentCount() % int(beat.calcOneBar_Frame()) == 0)
		{
			//�A�[�L�^�C�v����
		}
	}
};