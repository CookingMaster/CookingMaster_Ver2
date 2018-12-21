/**
* @file CalcurationBeat.hpp
* @brief BPMから拍の長さを計算する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <assert.h>

class CalcurationBeat
{
private:
	const float bpm_ = 0;

public:
	CalcurationBeat() {}
	CalcurationBeat(int bpm) : bpm_((float)bpm) {}
	CalcurationBeat(float bpm) : bpm_(bpm){}

	/**
	* @brief 一小節(全音符)の長さを計算してフレームで返す
	* @return float 一小節(全音符)の長さ
	*/
	[[nodiscard]]float calcOneBar_Frame()
	{
		return (60.f / bpm_) * 240.f;
	}
	/**
	* @brief 一小節(全音符)の長さを計算してミリ秒で返す
	* @return float 一小節(全音符)の長さ
	*/
	[[nodiscard]]float calcOneBar_Millisecond()
	{
		return (60.f / bpm_) * 4000.f;
	}

	/**
	* @brief 指定した拍の長さを計算してフレームで返す
	* @param beat 拍数
	* @return float 拍の長さ
	*/
	[[nodiscard]]float calcNote_Frame(float beat)
	{
		assert(beat >= 1.f && "The beat is incorrect!");
		return calcOneBar_Frame() / beat;
	}
	/**
	* @brief 指定音符の長さを計算してミリ秒で返す
	* @param beat 拍数
	* @return float 拍の長さ
	*/
	[[nodiscard]]float calcNote_Millisecond(float beat)
	{
		assert(beat >= 1.f && "The beat is incorrect!");
		return calcOneBar_Millisecond() / beat;
	}
	/**
	* @brief ミリ秒をフレーム数に変換する
	* @param ms ミリ秒
	* @return float フレーム数
	*/
	float millisecondToFrame(float ms)
	{
		return (ms / 1000.f) * 60.f;
	}
	/**
	* @brief フレーム数をミリ秒に変換する
	* @param frame フレーム数
	* @return float ミリ秒
	*/
	float frameToMillisecond(float ms)
	{
		return (ms / 1000.f) * 60.f;
	}
};