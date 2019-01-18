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
#include "../Class/Sound.hpp"

class NotesCreator
{
#undef max
private:
	int bpm_ = 0;			//BPM
	int beat_ = 0;			//拍子
	int offsetTime_ = 0;	//オフセット時間
	float goalTime_ = 0.f;	//次にtrueとなる時間
	Sound sound_;			//音楽
	Counter cntBar_;		//ノーツ数
	Vec2 targetPos[2];		//ノーツの到着する座標

public:
	NotesCreator(const std::string& soundName, const Vec2& targetL, const Vec2& targetR):
		sound_(soundName)
	{
		targetPos[0] = targetL;
		targetPos[1] = targetR;
	}

	/**
	* @brief 各種データ設定
	*/
	void set(int bpm, int beat, int offsetTime)
	{
		bpm_ = bpm;
		beat_ = beat;
		offsetTime_ = offsetTime;
		CalcurationBeat cbeat(bpm_, beat_);
		goalTime_ = cbeat.calcNote_Millisecond(1.f);
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
		if (sound_.getCurrentTime() > goalTime_)
		{
			CalcurationBeat cbeat(bpm_, beat_);
			goalTime_ += cbeat.calcNote_Millisecond(1.f);
			//ノーツ生成
			createNotes(notesData, scoreData, entityManager);
		}
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

			Vec2 tp;
			switch (scoreData[nextBar][i].dir)
			{
			case ECS::Direction::Dir::L:	//左から飛んでくるノーツ
				tp = targetPos[0];
				break;

			case ECS::Direction::Dir::R:	//右から飛んでくるノーツ
				tp = targetPos[1];
				break;
			}
			//ノーツを生成
			ECS::NotesArcheType::CreateNotes(
				nd,
				scoreData[nextBar][i].dir,
				waitTime,
				arrivalBeatTime,
				tp,
				entityManager);
		}

		++cntBar_;
	}
};