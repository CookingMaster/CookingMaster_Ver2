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
	int bpm;
	int offsetTime;
	std::vector<NotesData> notesData;
	ScoreData scoreData;

public:
	//���ʃf�[�^��ǂݍ���
	void loadMusicScoreData(const std::string& musicScoreDataPath)
	{
		std::string tmpstr;	//��������ꎞ�I�Ɋi�[���邽�߂̂��
		std::ifstream fin(musicScoreDataPath);
		assert(fin && "ScoreDataFile was not found!");

		//BPM�ǂݍ���
		int bpm;
		fin >> tmpstr >> bpm;

		//OffsetTime�ǂݍ���
		int offset;
		fin >> tmpstr >> offset;

		//NotesData�ǂݍ���
		addRestNotes();
		while (std::getline(fin, tmpstr))
		{
			if (tmpstr == "#START") break;
			loadNotesData(tmpstr);
		}

		//���ʓǂݍ���
		while(std::getline(fin, tmpstr))
		{
			if (tmpstr == "#END") break;
			addScoreData(tmpstr);
		}

		fin.close();
	}

	//�S�Ẵf�[�^���폜����
	void allDataClear()
	{
		bpm = 0;
		offsetTime = 0.f;

		notesData.clear();
		notesData.shrink_to_fit();

		for (auto& it : scoreData)
		{
			it.clear();
			it.shrink_to_fit();
		}
		scoreData.clear();
		scoreData.shrink_to_fit();
	}

	[[nodiscard]]int GetBPM()
	{
		return bpm;
	}

	[[nodiscard]]float GetOffsetTime()
	{
		return offsetTime;
	}

	[[nodiscard]]const std::vector<NotesData>& GetNotesData()
	{
		return notesData;
	}

	[[nodiscard]]const ScoreData& GetScoreData()
	{
		return scoreData;
	}

private:
	//�x���̃m�[�c�f�[�^��ǉ�����
	void addRestNotes()
	{
		notesData.emplace_back();
		notesData.back().imageName = "rest";
		notesData.back().seName = "rest";
		notesData.back().arrivalBeatTime = 0;
		for (int i = 0; i < 4; ++i)
		{
			notesData.back().hitJudge[i] = 0.f;
		}
		notesData.shrink_to_fit();
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

		notesData.emplace_back();
		fin >> notesData.back().imageName
			>> notesData.back().seName
			>> notesData.back().arrivalBeatTime
			>> notesData.back().hitJudge[0]
			>> notesData.back().hitJudge[1]
			>> notesData.back().hitJudge[2]
			>> notesData.back().hitJudge[3];
		notesData.shrink_to_fit();

		fin.close();
	}

	//���ʃf�[�^��ǉ�����
	void addScoreData(const std::string& score)
	{
		std::stringstream ss;
		ss << score;

		scoreData.emplace_back();
		scoreData.shrink_to_fit();

		std::vector<std::string> idstr;
		while (true)
		{
			idstr.emplace_back();
			ss >> idstr.back();	//��������擾
			if (idstr.back() == ",") break;	//�R�������������玟��
		}
		idstr.pop_back();
		scoreData.back().resize(idstr.size());

		int id;
		for (unsigned int i = 0; i < idstr.size(); ++i)
		{
			id = std::stoi(idstr[i]);

			scoreData.back()[i].notesID = abs(id);	//�擾�����m�[�c�ԍ�����
			if (id > 0) scoreData.back()[i].dir = OneNoteData::Direction::RIGHT;	//���̐���������E����
			else scoreData.back()[i].dir = OneNoteData::Direction::LEFT;			//���̐��������獶����
		}
	}
};