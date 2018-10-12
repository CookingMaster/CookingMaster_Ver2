/**
* @file NotesCreator.hpp
* @brief データを受け取り、それを基にノーツを生成する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include "NotesAndScoreData.hpp"
#include "../Utility/Counter.hpp"

class NotesCreator
{
private:
	int bpm;
	int offsetTime;
	TCounter<int> counter;

public:
	void ResetData(int setBPM, int setOffsetTime)
	{
		bpm = setBPM;
		offsetTime = setOffsetTime;
	}
};