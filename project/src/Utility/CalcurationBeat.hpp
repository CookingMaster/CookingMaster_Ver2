/**
* @file CalcurationBeat.hpp
* @brief BPMから各種音符の長さを計算する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <assert.h>

class CalcurationBeat
{
private:
	const float bpm;

public:
	CalcurationBeat(float setBpm) : bpm(setBpm){}

	//一小節(全音符)の長さを計算してフレームで返す
	[[nodiscard]]float CalcOneBar_Frame()
	{
		return (60.f / bpm) * 240.f;
	}
	//一小節(全音符)の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcOneBar_Millisecond()
	{
		return (60.f / bpm) * 4000.f;
	}

	//指定音符の長さを計算してフレームで返す
	[[nodiscard]]float CalcNote_Frame(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return CalcOneBar_Frame() / beat;
	}
	//指定音符の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcNote_Millisecond(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return CalcOneBar_Millisecond() / beat;
	}
};