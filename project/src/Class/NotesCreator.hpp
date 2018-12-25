/**
* @file NotesCreator.hpp
* @brief データを受け取り、それを基にノーツを生成する
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
	int bpm_ = 0;			//BPM
	int beat_ = 0;			//拍子
	int offsetTime_ = 0;	//オフセット時間
	TCounter<int> cntTime_;	//時間計測
	TCounter<int> cntBar_;	//小節の計測

public:
	/**
	* @brief BPM, OffsetTimeを設定し、経過時間を0にする
	* @param bpm BPM
	* @param offsetTime オフセット時間
	*/
	void resetData(int bpm, int beat, int offsetTime)
	{
		bpm_ = bpm;
		beat_ = beat;
		offsetTime_ = offsetTime;
		cntTime_.reset();
		cntBar_.reset();
	}

	/**
	* @brief 更新処理
	* @param notesData 使用するノーツのデータ
	* @param 譜面データ
	* @param entityManager エンティティマネージャ
	*/
	void run(const std::vector<NotesData>& notesData, const MusicData& scoreData, ECS::EntityManager& entityManager)
	{
		CalcurationBeat beat(bpm_, beat_);

		//一小節毎にノーツを生成する
		if (cntTime_.getCurrentCount() % int(beat.calcOneBar_Frame()) == 0)
		{
			//ノーツ生成
			createNotes(notesData, scoreData, entityManager);
		}
		++cntTime_;
	}

private:
	//ノーツを生成する
	void createNotes(const std::vector<NotesData>& notesData, const MusicData& scoreData, ECS::EntityManager& entityManager)
	{
		//次の小節の譜面を見る
		int nextBar = cntBar_.getCurrentCount() + 1;
		if ((unsigned int)nextBar >= scoreData.size()) return;
		//その小節内で生成されるノーツ数から音の長さを計算
		CalcurationBeat beat(bpm_, beat_);
		float noteFlame = beat.calcNote_Frame(float(scoreData[nextBar].size()));

		for (unsigned int i = 0; i < scoreData[nextBar].size(); ++i)
		{
			NotesData nd = notesData[scoreData[nextBar][i].notesID];
			if (nd.imageName == "rest")
				continue;

			//目標地点に到達するまでの時間を計算
			float arrivalBeatTime = beat.calcNote_Frame(float(nd.arrivalBeatTime));
			//そのノーツが画面内に出現するまでの待ち時間を計算
			float waitTime = beat.calcOneBar_Frame() * 2.f - (float(scoreData[nextBar].size() - i) * noteFlame) - arrivalBeatTime;

			//生成
			ECS::NotesArcheType::CreateNotes(
				nd,
				scoreData[nextBar][i].dir,
				waitTime,
				arrivalBeatTime,
				Vec2(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f),
				entityManager);
		}

		++cntBar_;
	}
};