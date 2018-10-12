/**
* @file CalcurationBeat.hpp
* @brief BPMから各種音符の長さを計算する
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once

class CalcurationBeat
{
private:
	float bpm;

public:
	CalcurationBeat(float setBpm) : bpm(setBpm){}

	//一拍(4分音符)の長さを計算してフレームで返す
	[[nodiscard]]float CalcOneBeat_Frame()
	{
		return (60.f / bpm) * 60.f;
	}
	//一拍(4分音符)の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcOneBeat_Millisecond()
	{
		return (60.f / bpm) * 1000.f;
	}
	
	//一小節(全音符)の長さを計算してフレームで返す
	[[nodiscard]]float CalcOneBar_Frame()
	{
		return CalcOneBar_Frame() * 4.f;
	}
	//一小節(全音符)の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcOneBar_Millisecond()
	{
		return CalcOneBeat_Millisecond() * 4.f;
	}

	//2分音符の長さを計算してフレームで返す
	[[nodiscard]]float CalcHalfNote_Frame()
	{
		return CalcOneBar_Frame() * 2.f;
	}
	//2分音符の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcHalfNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() * 2.f;
	}

	//8分音符の長さを計算してフレームで返す
	[[nodiscard]]float CalcEighthNote_Frame()
	{
		return CalcOneBar_Frame() / 2.f;
	}
	//8分音符の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcEighthNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() / 2.f;
	}

	//16分音符の長さを計算してフレームで返す
	[[nodiscard]]float CalcSixteenthNote_Frame()
	{
		return CalcOneBar_Frame() / 4.f;
	}
	//16分音符の長さを計算してミリ秒で返す
	[[nodiscard]]float CalcSixteenthNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() / 4.f;
	}
};