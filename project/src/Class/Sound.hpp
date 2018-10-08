/**
* @file Sound.hpp
* @brief DXライブラリのサウンド関数をラップします
* @author tonarinohito
* @date 2018/10/08
*/
#pragma once
#include "ResourceManager.hpp"

//!サウンド操作クラスです
class Sound final
{
private:
	std::string name_;
	int handle_;
public:
	//!コンストラクタで登録したサウンドハンドル名を指定します
	Sound(const std::string& soundName)
	{
		assert(ResourceManager::GetSound().hasHandle(soundName));
		handle_ = ResourceManager::GetSound().getHandle(soundName);
		name_ = soundName;
	}
	//!サウンドを再生します
	void play(bool isLoop)
	{
		if (isLoop)
		{
			PlaySoundMem(handle_, DX_PLAYTYPE_LOOP);
		}
		else
		{
			PlaySoundMem(handle_, DX_PLAYTYPE_BACK);
		}
	}
	//!サウンドの再生中を止めます
	void stop()
	{
		StopSoundMem(handle_);
	}
	//!サウンドが再生中か取得します
	[[nodiscard]] const bool isPlay() const
	{
		switch (CheckSoundMem(handle_))
		{
		case 0: return false;
		case 1: return true;
		}
	}
	//!サウンドの現在の再生位置をミリ秒単位で取得します
	[[nodiscard]] const int getCurrentTime() const
	{
		return GetSoundCurrentTime(handle_);
	}
	//!サウンドの総時間をミリ秒単位で取得します
	[[nodiscard]] const int getTotalTime() const 
	{
		return GetSoundTotalTime(handle_);
	}
	/**
	* @brief サウンドのパンを設定します
	* @param  panPosition 音の位置(-255~255)
	* @return ハンドルが存在したらtrue
	*/
	void setPan(const int panPosition)
	{
		ChangePanSoundMem(panPosition,handle_);
	}
	//!このサウンドの音量を0.0f~1.fで指定
	void setGain(float gain)
	{
		ChangeVolumeSoundMem(int(255 * gain), handle_);
	}
	//!すべてのサウンドの音量を0.0f~1.fで指定
	static void SetAllGain(float gain)
	{
		auto& sounds = ResourceManager::GetSound().getSoundMap();
		for (auto&[key, val] : sounds)
		{
			ChangeVolumeSoundMem(int(255 * gain), val);
		}
	}
	
};