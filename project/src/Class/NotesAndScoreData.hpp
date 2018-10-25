/**
* @file MusicScoreLoader.hpp
* @brief ノーツ・譜面に関する構造体
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <string>
#include <vector>
#include "../Components/BasicComponents.hpp"

//ノーツに使用される各種データ
struct NotesData
{
	std::string imagePath;	//�摜�ւ̃p�X
	std::string imageName;	//�g�p����摜��
	std::string seName;		//�؂������̌��ʉ���
	int arrivalBeatTime;	//�}�[�J�[�ւ̓��B����
	float hitJudge[4];		//�ڐG���莞��(MISS����]��4�i�K��)
	
	int xsize;		//�摜��X�����̃T�C�Y
	int ysize;		//�摜��Y�����̃T�C�Y
	int animFlame;	//�A�j���[�V�����̃t���[����
	int xnum;		//�A�j���[�V������X�����̖���
	int ynum;		//�A�j���[�V������Y�����̖���
};

//一つの音符のデータ
struct OneNoteData
{
	int notesID;				//�m�[�c�̔ԍ�
	ECS::Direction::Dir dir;	//���ł������
};

//譜面データ
typedef std::vector<std::vector<OneNoteData>> ScoreData;
