/**
* @file NotesLoader.hpp
* @brief ノーツの出現タイミングやデータを読み込み、外部に提供する
* @author feveleK5563
* @date 2018/10/11
*/

#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <vector>

//ノーツに使用される各種データ
struct NotesData
{
	std::string imageName;	//使用する画像名
	std::string seName;		//切った時の効果音名
	int arrivalBeatTime;	//マーカーへの到達拍数
	float hitJudge[4];		//接触判定時間(MISSを除く評価4段階分)
};

//一つの音符のデータ
struct ScoreNoteData
{
	int notesID;								//ノーツの番号
	enum class Direction { LEFT, RIGHT } dir;	//飛んでくる方向
};

//譜面データ
typedef std::vector<std::vector<ScoreNoteData>> ScoreData;


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
		if (!fin) assert(false && "ScoreData was not found!");

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

	int GetBPM()
	{
		return bpm;
	}

	float GetOffsetTime()
	{
		return offsetTime;
	}

	const std::vector<NotesData>& GetNotesData()
	{
		return notesData;
	}

	const ScoreData& GetScoreData()
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
		if (!fin) assert(false && "NotesDataPath was not found!");

		//読み込む
		notesData.emplace_back();
		fin >> notesData.back().imageName
			>> notesData.back().seName
			>> notesData.back().arrivalBeatTime
			>> notesData.back().hitJudge[0]
			>> notesData.back().hitJudge[1]
			>> notesData.back().hitJudge[2]
			>> notesData.back().hitJudge[3]
			>> notesData.back().hitJudge[4];
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
			if (idstr.back() == ",") break;	//コロンだったら次へ
		}
		idstr.pop_back();
		scoreData.back().resize(idstr.size());

		int id;
		for (unsigned int i = 0; i < idstr.size(); ++i)
		{
			id = std::stoi(idstr[i]);

			scoreData.back()[i].notesID = abs(id);	//取得したノーツ番号を代入
			if (id > 0) scoreData.back()[i].dir = ScoreNoteData::Direction::RIGHT;	//正の数だったら右から
			else scoreData.back()[i].dir = ScoreNoteData::Direction::LEFT;			//負の数だったら左から
		}
	}
};