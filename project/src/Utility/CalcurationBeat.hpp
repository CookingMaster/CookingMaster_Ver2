/**
* @file CalcurationBeat.hpp
* @brief BPM����e�퉹���̒������v�Z����
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

	//�ꏬ��(�S����)�̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcOneBar_Frame()
	{
		return (60.f / bpm) * 240.f;
	}
	//�ꏬ��(�S����)�̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcOneBar_Millisecond()
	{
		return (60.f / bpm) * 4000.f;
	}

	//�w�艹���̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcNote_Frame(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return CalcOneBar_Frame() / beat;
	}
	//�w�艹���̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcNote_Millisecond(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return CalcOneBar_Millisecond() / beat;
	}
};