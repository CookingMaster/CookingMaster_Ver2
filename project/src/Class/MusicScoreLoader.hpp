/**
* @file MusicScoreLoader.hpp
* @brief �m�[�c�̏o���^�C�~���O��f�[�^��ǂݍ��݁A�O���ɒ񋟂���
* @author feveleK5563
* @date 2018/10/11
*/

#pragma once
#include <sstream>
#include <fstream>
#include <assert.h>
#include "NotesAndScoreData.hpp"

//-----------------------------------------------------------------------------
//�ǂݍ���&�񋟂���
class MusicScoreLoader
{
private:
	int bpm_;
	int offsetTime_;
	std::vector<NotesData> notesData_;
	ScoreData scoreData_;

public:
	/**
	* @brief ���ʃf�[�^��ǂݍ���
	* @param musicScoreDataPath ���ʏ��t�@�C���ւ̃p�X
	*/
	void loadMusicScoreData(const std::string& musicScoreDataPath)
	{
		std::string tmpstr;	//��������ꎞ�I�Ɋi�[���邽�߂̂��
		std::ifstream fin(musicScoreDataPath);
		assert(fin && "ScoreDataFile was not found!");

		//BPM�ǂݍ���
		fin >> tmpstr >> bpm_;

		//OffsetTime�ǂݍ���
		fin >> tmpstr >> offsetTime_;

		//NotesData�ǂݍ���
		addRestNotes();
		while (std::getline(fin, tmpstr))
		{
			if (tmpstr == "#START") break;
			loadNotesData(tmpstr);
		}

		//���ʓǂݍ���
		addRestScoreData();
		while(std::getline(fin, tmpstr))
		{
			if (tmpstr == "#END") break;
			addScoreData(tmpstr);
		}

		fin.close();
	}

	/**
	* @brief �S�Ẵf�[�^���폜����
	*/
	void allDataClear()
	{
		bpm_ = 0;
		offsetTime_ = 0.f;

		notesData_.clear();
		notesData_.shrink_to_fit();

		for (auto& it : scoreData_)
		{
			it.clear();
			it.shrink_to_fit();
		}
		scoreData_.clear();
		scoreData_.shrink_to_fit();
	}

	/**
	* @brief BPM���擾����
	* @return int BPM
	*/
	[[nodiscard]]int GetBPM()
	{
		return bpm_;
	}
	/**
	* @brief �I�t�Z�b�g�̒l(�t���[��)���擾����
	* @return float �I�t�Z�b�g����
	*/
	[[nodiscard]]float GetOffsetTime()
	{
		return offsetTime_;
	}
	/**
	* @brief �g�p����m�[�c�̃f�[�^���擾����
	* @return const std::vector<NotesData>& �g�p����m�[�c�̃f�[�^
	*/
	[[nodiscard]]const std::vector<NotesData>& GetNotesData()
	{
		return notesData_;
	}
	/**
	* @brief ���ʃf�[�^���擾����
	* @return const ScoreData& ���ʃf�[�^
	*/
	[[nodiscard]]const ScoreData& GetScoreData()
	{
		return scoreData_;
	}

private:
	//�x���̃m�[�c�f�[�^��ǉ�����
	void addRestNotes()
	{
		notesData_.emplace_back();
		notesData_.back().imageName = "rest";
		notesData_.back().seName = "rest";
		notesData_.back().arrivalBeatTime = 0;
		for (int i = 0; i < 4; ++i)
		{
			notesData_.back().hitJudge[i] = 0.f;
		}
		notesData_.shrink_to_fit();
	}

	//�m�[�c�f�[�^��ǂݍ���
	void loadNotesData(const std::string& note)
	{
		std::stringstream ss;
		ss << note;
		int num; std::string notesDataPath;
		ss >> num >> notesDataPath;

		std::ifstream fin(notesDataPath);
		assert(fin && "NotesDataFile was not found!");

		notesData_.emplace_back();
		fin >> notesData_.back().imageName
			>> notesData_.back().seName
			>> notesData_.back().arrivalBeatTime
			>> notesData_.back().hitJudge[0]
			>> notesData_.back().hitJudge[1]
			>> notesData_.back().hitJudge[2]
			>> notesData_.back().hitJudge[3];
		notesData_.shrink_to_fit();

		fin.close();
	}

	//�S�x���݂̂̏��߂𕈖ʃf�[�^�ɒǉ�����
	void addRestScoreData()
	{
		scoreData_.emplace_back();
		scoreData_.shrink_to_fit();

		scoreData_.back().resize(1);
		scoreData_.back().back() = {0, OneNoteData::Direction::LEFT};
	}

	//���ʃf�[�^��ǉ�����
	void addScoreData(const std::string& score)
	{
		std::stringstream ss;
		ss << score;

		scoreData_.emplace_back();
		scoreData_.shrink_to_fit();

		std::vector<std::string> idstr;
		while (true)
		{
			idstr.emplace_back();
			ss >> idstr.back();	//��������擾
			if (idstr.back() == ",") break;	//�R�������������玟��
		}
		idstr.pop_back();
		scoreData_.back().resize(idstr.size());

		int id;
		for (unsigned int i = 0; i < idstr.size(); ++i)
		{
			id = std::stoi(idstr[i]);

			scoreData_.back()[i].notesID = abs(id);	//�擾�����m�[�c�ԍ�����
			if (id > 0) scoreData_.back()[i].dir = OneNoteData::Direction::RIGHT;	//���̐���������E����
			else scoreData_.back()[i].dir = OneNoteData::Direction::LEFT;			//���̐��������獶����
		}
	}
};