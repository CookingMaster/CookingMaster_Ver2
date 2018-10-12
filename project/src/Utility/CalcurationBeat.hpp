/**
* @file CalcurationBeat.hpp
* @brief BPM���甏�̒������v�Z����
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <assert.h>

class CalcurationBeat
{
private:
	const float bpm_;

public:
	CalcurationBeat(float bpm) : bpm_(bpm){}

	//�ꏬ��(�S����)�̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float calcOneBar_Frame()
	{
		return (60.f / bpm_) * 240.f;
	}
	//�ꏬ��(�S����)�̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float calcOneBar_Millisecond()
	{
		return (60.f / bpm_) * 4000.f;
	}

	//�w�艹���̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float calcNote_Frame(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return calcOneBar_Frame() / beat;
	}
	//�w�艹���̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float calcNote_Millisecond(float beat)
	{
		assert(beat > 0.f && "The beat is incorrect!");
		return calcOneBar_Millisecond() / beat;
	}
};