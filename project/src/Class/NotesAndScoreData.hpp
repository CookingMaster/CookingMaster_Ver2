/**
* @file MusicScoreLoader.hpp
* @brief �m�[�c�E���ʂɊւ���\����
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <string>
#include <vector>
#include "../Components/BasicComponents.hpp"

//�m�[�c�Ɏg�p�����e��f�[�^
struct NotesData
{
	std::string imagePath;	//�摜�ւ̃p�X
	std::string imageName;	//�g�p����摜��
	std::string seName;		//�؂������̌��ʉ���
	int arrivalBeatTime;	//�}�[�J�[�ւ̓��B����
	float hitJudge[4];		//�ڐG���莞��(MISS�������]��4�i�K��)
	
	int xsize;		//�摜��X�����̃T�C�Y
	int ysize;		//�摜��Y�����̃T�C�Y
	int animFlame;	//�A�j���[�V�����̃t���[����
	int xnum;		//�A�j���[�V������X�����̖���
	int ynum;		//�A�j���[�V������Y�����̖���
};

//��̉����̃f�[�^
struct OneNoteData
{
	int notesID;				//�m�[�c�̔ԍ�
	ECS::Direction::Dir dir;	//���ł������
};

//���ʃf�[�^
typedef std::vector<std::vector<OneNoteData>> ScoreData;
