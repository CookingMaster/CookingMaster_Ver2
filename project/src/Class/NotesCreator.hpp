/**
* @file NotesCreator.hpp
* @brief データを受け取り、それを基にノーツを生成する
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
	//BPM, OffsetTimeをリセットする
	void resetData(int bpm, int offsetTime)
	{
		bpm_ = bpm;
		offsetTime_ = offsetTime;
		counter_.reset();
	}

	//更新処理
	void run(const std::vector<NotesData>& notesData, const ScoreData& scoreData)
	{
		CalcurationBeat beat((float)bpm_);

		++counter_;
		//一小節毎にノーツを生成する
		if (counter_.getCurrentCount() % int(beat.calcOneBar_Frame()) == 0)
		{
			//アーキタイプ生成
		}
	}
};