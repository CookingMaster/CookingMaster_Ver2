﻿/**
* @file MusicScoreLoader.hpp
* @brief ノーツ・譜面に関する構造体
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <string>
#include <vector>
#include <array>
#include "../Components/BasicComponents.hpp"

//アニメーションのスタート位置と枚数
struct AnimSheetData final
{
	int xmin, ymin, xmax, ymax;
};

//ノーツに使用される各種データ
struct NotesData final
{
	std::string imagePath;	//画像のパス
	std::string imageName;	//画像名
	int dirtyID;			//グチャってなったときの画像番号
	std::string seName;		//SE名
	int arrivalBeatTime;	//ターゲットまでの到達時間
	std::array<float, 4> hitJudge{};		//判定時間
	
	int xsize;		//画像のX方向のサイズ
	int ysize;		//画像のY方向のサイズ
	int animFlame;	//アニメーションのフレーム数
	int xnum;		//アニメーションのX方向の枚数
	int ynum;		//アニメーションのY方向の枚数
	std::array<AnimSheetData, 3> animSData{};	//通常、切断、落下のアニメーション
};

//一つの音符のデータ
struct OneNoteData final
{
	int notesID;				//ノーツ番号
	ECS::Direction::Dir dir;	//ノーツの向き
};

//譜面データ
typedef std::vector<std::vector<OneNoteData>> MusicData;
