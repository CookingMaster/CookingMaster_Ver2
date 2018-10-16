/**
* @file NotesCreator.hpp
* @brief �f�[�^���󂯎��A�������Ƀm�[�c�𐶐�����
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "../Utility/Counter.hpp"
#include "../Utility/CalcurationBeat.hpp"
#include "../ArcheType/NotesArcheType.hpp"

class NotesCreator
{
#undef max
private:
	int bpm_;					//BPM
	int offsetTime_;			//�I�t�Z�b�g����
	TCounter<int> cntTime_;		//���Ԍv��
	TCounter<int> cntBar_;		//���߂̌v��

public:
	/**
	* @brief BPM, OffsetTime��ݒ肵�A�o�ߎ��Ԃ�0�ɂ���
	* @param bpm BPM
	* @param offsetTime �I�t�Z�b�g����
	*/
	void resetData(int bpm, int offsetTime)
	{
		bpm_ = bpm;
		offsetTime_ = offsetTime;
		cntTime_.reset();
		cntBar_.reset();
	}

	/**
	* @brief �X�V����
	* @param notesData �g�p����m�[�c�̃f�[�^
	* @param scoreData ���ʃf�[�^
	* @param entityManager �G���e�B�e�B�}�l�[�W��
	*/
	void run(const std::vector<NotesData>& notesData, const ScoreData& scoreData, ECS::EntityManager& entityManager)
	{
		CalcurationBeat beat((float)bpm_);

		//�ꏬ�ߖ��Ƀm�[�c�𐶐�����
		if (cntTime_.getCurrentCount() % int(beat.calcOneBar_Frame()) == 0)
		{
			//�m�[�c����
			createNotes(notesData, scoreData, entityManager);
		}
		++cntTime_;
	}

private:
	//�m�[�c�𐶐�����
	void createNotes(const std::vector<NotesData>& notesData, const ScoreData& scoreData, ECS::EntityManager& entityManager)
	{
		//���̏��߂̕��ʂ�����
		int nextBar = cntBar_.getCurrentCount() + 1;
		if ((unsigned int)nextBar >= scoreData.size()) return;
		
		//���̏��ߓ��Ő��������m�[�c�����特�̒������v�Z
		CalcurationBeat beat((float)bpm_);
		float noteFlame = beat.calcNote_Frame(float(scoreData[nextBar].size()));

		for (unsigned int i = 0; i < scoreData[nextBar].size(); ++i)
		{
			NotesData nd = notesData[scoreData[nextBar][i].notesID];
			if (nd.imageName == "rest")
				continue;

			float arrivalBeatTime = beat.calcNote_Frame(nd.arrivalBeatTime);
			//���̃m�[�c����ʓ��ɏo������܂ł̑҂����Ԃ��v�Z
			float waitTime = beat.calcOneBar_Frame() * 2.f - (float(scoreData[nextBar].size() - i) * noteFlame) - arrivalBeatTime;

			//����
			ECS::NotesArcheType::CreateNotes(
				nd,
				scoreData[nextBar][i].dir,
				waitTime,
				entityManager);
		}

		++cntBar_;
	}
};