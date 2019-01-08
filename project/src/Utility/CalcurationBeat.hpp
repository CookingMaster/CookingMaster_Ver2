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
	const float beat_ = 1.f;
	const float bpm_ = 0.f;

public:
	CalcurationBeat() {}
	//bpm：BPM、beat：拍子
	CalcurationBeat(int bpm, int beat) :
		bpm_((float)bpm), beat_((float)beat) {}
	//bpm：BPM、beat：拍子
	CalcurationBeat(float bpm, float beat) :
		bpm_(bpm), beat_(beat) {}

	/**
	* @brief 一小節(全音符)の長さを計算してフレームで返す
	* @return float 一小節(全音符)の長さ
	*/
	[[nodiscard]]float calcOneBar_Frame()
	{
		return (60.f / bpm_) * (60.f * beat_);
	}
	/**
	* @brief 一小節(全音符)の長さを計算してミリ秒で返す
	* @return float 一小節(全音符)の長さ
	*/
	[[nodiscard]]float calcOneBar_Millisecond()
	{
		return (60.f / bpm_) * (1000.f * beat_);
	}

	/**
	* @brief 指定した拍の長さを計算してフレームで返す
	* @param num 拍数
	* @return float 拍の長さ
	*/
	[[nodiscard]]float calcNote_Frame(float num)
	{
		assert(num >= 1.f && "The beat is incorrect!");
		return calcOneBar_Frame() / num;
	}
	/**
	* @brief 指定音符の長さを計算してミリ秒で返す
	* @param num 拍数
	* @return float 拍の長さ
	*/
	[[nodiscard]]float calcNote_Millisecond(float num)
	{
		assert(num >= 1.f && "The beat is incorrect!");
		return calcOneBar_Millisecond() / num;
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
	float frameToMillisecond(float frame)
	{
		return (frame / 1000.f) * 60.f;
	}
};