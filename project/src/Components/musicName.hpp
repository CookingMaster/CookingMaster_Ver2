/**
* @file musicName.hpp
* @brief セレクト画面で使う曲名格納用データです
* @author tonarinohito
* @date 2018/12/10
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../Utility/Vec.hpp"
#include <DxLib.h>

namespace ECS
{
	/*
	@brief 曲名保存用のデータです。拡張子まで指定してください
	*/
	struct MusicName final : public ComponentData
	{
		std::string val = "";
		MusicName(const std::string& name) :
			val(name) {}
	};
}
