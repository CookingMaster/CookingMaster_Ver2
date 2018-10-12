/**
* @file MusicScoreLoader.hpp
* @brief ノーツの出現タイミングやデータを読み込み、外部に提供する
* @author feveleK5563
* @date 2018/10/11
*/

#pragma once
#include <sstream>
#include <fstream>
#include <assert.h>
#include "NotesAndScoreData.hpp"

//-----------------------------------------------------------------------------
//読み込み&提供くん
class MusicScoreLoader
{
private:
	int bpm;
	int offsetTime;
	std::vector<NotesData> notesData;
	ScoreData scoreData;

public:
	//譜面データを読み込む
	void loadMusicScoreData(const std::string& musicScoreDataPath)
	{
		std::string tmpstr;	//文字列を一時的に格納するためのやつ
		std::ifstream fin(musicScoreDataPath);
		assert(fin && "ScoreDataFile was not found!");

		//BPM読み込み
		int bpm;
		fin >> tmpstr >> bpm;

		//OffsetTime読み込み
		int offset;
		fin >> tmpstr >> offset;

		//NotesData読み込み
		addRestNotes();
		while (std::getline(fin, tmpstr))
		{
			if (tmpstr == "#START") break;
			loadNotesData(tmpstr);
		}

		//譜面読み込み
		while(std::getline(fin, tmpstr))
		{
			if (tmpstr == "#END") break;
			addScoreData(tmpstr);
		}

		fin.close();
	}

	//全てのデータを削除する
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
	//休符のノーツデータを追加する
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

	//ノーツデータを読み込む
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

	//譜面データを追加する
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
			ss >> idstr.back();	//文字列を取得
			if (idstr.back() == ",") break;	//コロンがあったら次へ
		}
		idstr.pop_back();
		scoreData.back().resize(idstr.size());

		int id;
		for (unsigned int i = 0; i < idstr.size(); ++i)
		{
			id = std::stoi(idstr[i]);

			scoreData.back()[i].notesID = abs(id);	//取得したノーツ番号を代入
			if (id > 0) scoreData.back()[i].dir = OneNoteData::Direction::RIGHT;	//正の数だったら右から
			else scoreData.back()[i].dir = OneNoteData::Direction::LEFT;			//負の数だったら左から
		}
	}
};