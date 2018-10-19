/**
* @file MusicScoreLoader.hpp
* @brief �m�[�c�E���ʂɊւ���\����
* @author feveleK5563
* @date 2018/10/12
*/

#pragma once
#include <string>
#include <vector>

//�m�[�c�Ɏg�p�����e��f�[�^
struct NotesData
{
	std::string imagePath;	//�摜�ւ̃p�X
	std::string imageName;	//�g�p����摜��
	std::string seName;		//�؂������̌��ʉ���
	int arrivalBeatTime;	//�}�[�J�[�ւ̓��B����
	float hitJudge[4];		//�ڐG���莞��(MISS�������]��4�i�K��)
};

//��̉����̃f�[�^
struct OneNoteData
{
	int notesID;								//�m�[�c�̔ԍ�
	enum class Direction { LEFT, RIGHT } dir;	//���ł������
};

//���ʃf�[�^
typedef std::vector<std::vector<OneNoteData>> ScoreData;
