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
#include "ResourceManager.hpp"

//-----------------------------------------------------------------------------
//読み込み&提供くん
class MusicScoreLoader
{
private:
	int bpm_;
	int offsetTime_;
	std::vector<NotesData> notesData_;
	ScoreData scoreData_;

public:
	/**
	* @brief 譜面データを読み込む
	* @param musicScoreDataPath 譜面情報ファイルへのパス
	*/
	void loadMusicScoreData(const std::string& musicScoreDataPath)
	{
		std::string tmpstr;	//文字列を一時的に格納するためのやつ
		std::ifstream fin(musicScoreDataPath);
		assert(fin && "ScoreDataFile was not found!");

		//BPM読み込み
		fin >> tmpstr >> bpm_;

		//OffsetTime読み込み
		fin >> tmpstr >> offsetTime_;

		//NotesData読み込み
		addRestNotes();
		while (std::getline(fin, tmpstr))
		{
			if (tmpstr == "" || tmpstr == "#NOTESDATA") continue;
			if (tmpstr == "#START") break;
			loadNotesData(tmpstr);
		}

		//譜面読み込み
		addRestScoreData();
		while(std::getline(fin, tmpstr))
		{
			if (tmpstr == "#END") break;
			addScoreData(tmpstr);
		}

		fin.close();
	}

	/**
	* @brief 全てのデータを削除する
	*/
	void allDataClear()
	{
		bpm_ = 0;
		offsetTime_ = 0;

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
	* @brief BPMを取得する
	* @return int BPM
	*/
	[[nodiscard]]int GetBPM()
	{
		return bpm_;
	}
	/**
	* @brief オフセットの値(フレーム)を取得する
	* @return float オフセット時間
	*/
	[[nodiscard]]int GetOffsetTime()
	{
		return offsetTime_;
	}
	/**
	* @brief 使用するノーツのデータを取得する
	* @return const std::vector<NotesData>& 使用するノーツのデータ
	*/
	[[nodiscard]]const std::vector<NotesData>& GetNotesData()
	{
		return notesData_;
	}
	/**
	* @brief 譜面データを取得する
	* @return const ScoreData& 譜面データ
	*/
	[[nodiscard]]const ScoreData& GetScoreData()
	{
		return scoreData_;
	}

private:
	//休符のノーツデータを追加する
	void addRestNotes()
	{
		notesData_.emplace_back();
		notesData_.back().imagePath = "rest";
		notesData_.back().imageName = "rest";
		notesData_.back().seName = "rest";
		notesData_.back().arrivalBeatTime = 0;
		for (int i = 0; i < 4; ++i)
		{
			notesData_.back().hitJudge[i] = 0.f;
		}
		notesData_.shrink_to_fit();
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

		notesData_.emplace_back();
		fin >> notesData_.back().imagePath
			>> notesData_.back().imageName
			>> notesData_.back().seName
			>> notesData_.back().arrivalBeatTime
			>> notesData_.back().hitJudge[0]
			>> notesData_.back().hitJudge[1]
			>> notesData_.back().hitJudge[2]
			>> notesData_.back().hitJudge[3];
		notesData_.shrink_to_fit();

		//リソースの読み込み
		ResourceManager::GetGraph().load(notesData_.back().imagePath, notesData_.back().imageName);	//Image
		//SE

		fin.close();
	}

	//全休符のみの小節を譜面データに追加する
	void addRestScoreData()
	{
		scoreData_.emplace_back();
		scoreData_.shrink_to_fit();

		scoreData_.back().resize(1);
		scoreData_.back().back() = {0, OneNoteData::Direction::LEFT};
	}

	//譜面データを追加する
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
			ss >> idstr.back();	//文字列を取得
			if (idstr.back() == ",") break;	//コロンがあったら次へ
		}
		idstr.pop_back();
		scoreData_.back().resize(idstr.size());

		int id;
		for (unsigned int i = 0; i < idstr.size(); ++i)
		{
			id = std::stoi(idstr[i]);

			scoreData_.back()[i].notesID = abs(id);	//取得したノーツ番号を代入
			if (id > 0) scoreData_.back()[i].dir = OneNoteData::Direction::RIGHT;	//正の数だったら右から
			else scoreData_.back()[i].dir = OneNoteData::Direction::LEFT;			//負の数だったら左から
		}
	}
};