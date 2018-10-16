/**
* @file MusicScoreLoader.hpp
* @brief ノーツ・譜面に関する構造体
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <string>
#include <vector>

//ノーツに使用される各種データ
struct NotesData
{
	std::string imagePath;	//画像へのパス
	std::string imageName;	//使用する画像名
	std::string seName;		//切った時の効果音名
	int arrivalBeatTime;	//マーカーへの到達拍数
	float hitJudge[4];		//接触判定時間(MISSを除く評価4段階分)
};

//一つの音符のデータ
struct OneNoteData
{
	int notesID;								//ノーツの番号
	enum class Direction { LEFT, RIGHT } dir;	//飛んでくる方向
};

//譜面データ
typedef std::vector<std::vector<OneNoteData>> ScoreData;
