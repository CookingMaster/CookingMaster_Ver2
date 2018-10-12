/**
* @file CalcurationBeat.hpp
* @brief BPM����e�퉹���̒������v�Z����
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

	//�ꔏ(4������)�̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcOneBeat_Frame()
	{
		return (60.f / bpm) * 60.f;
	}
	//�ꔏ(4������)�̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcOneBeat_Millisecond()
	{
		return (60.f / bpm) * 1000.f;
	}
	
	//�ꏬ��(�S����)�̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcOneBar_Frame()
	{
		return CalcOneBar_Frame() * 4.f;
	}
	//�ꏬ��(�S����)�̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcOneBar_Millisecond()
	{
		return CalcOneBeat_Millisecond() * 4.f;
	}

	//2�������̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcHalfNote_Frame()
	{
		return CalcOneBar_Frame() * 2.f;
	}
	//2�������̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcHalfNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() * 2.f;
	}

	//8�������̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcEighthNote_Frame()
	{
		return CalcOneBar_Frame() / 2.f;
	}
	//8�������̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcEighthNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() / 2.f;
	}

	//16�������̒������v�Z���ăt���[���ŕԂ�
	[[nodiscard]]float CalcSixteenthNote_Frame()
	{
		return CalcOneBar_Frame() / 4.f;
	}
	//16�������̒������v�Z���ă~���b�ŕԂ�
	[[nodiscard]]float CalcSixteenthNote_Millisecond()
	{
		return CalcOneBeat_Millisecond() / 4.f;
	}
};