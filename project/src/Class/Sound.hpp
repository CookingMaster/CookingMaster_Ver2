/**
* @file Sound.hpp
* @brief DXライブラリのサウンド関数をラップします
* @detail ResourceManagerで読み込んだハンドルが対象です
* @author tonarinohito
* @date 2018/10/08
*/
#pragma once
#include "ResourceManager.hpp"

//!すべてのサウンドの音量に対しての処理を行います
class MasterSound final
{
private:
	class Singleton final
	{
	private:
		float seGain_ = 1.0f;
		float bgmGain_ = 1.0f;
	public:
		//!BGM音量の読み込み
		const float getBGMGain() { return bgmGain_; }
		const float getSEGain() { return seGain_; }
		//!すべてのSEサウンドの音量を0.0f~1.fで指定
		void setAllSEGain(float gain)
		{
			if (gain >= 1.f || gain <= 0) { return; }
			seGain_ = gain;
		}
		//!すべてのBGMサウンドの音量を0.0f~1.fで指定
		void setAllBGMGain(float gain)
		{
			if (gain >= 1.f || gain <= 0) { return; }
			bgmGain_ = gain;
		}
		//!登録されているサウンドの更新を行います
		void update()
		{
			auto& sounds = ResourceManager::GetSound().getSoundMap();
			for (auto&[key, val] : sounds)
			{
				switch (val.second)
				{
				case SoundType::SE:
					ChangeVolumeSoundMem(int(255 * seGain_), val.first);
					break;
				case SoundType::BGM:
					ChangeVolumeSoundMem(int(255 * bgmGain_), val.first);
					break;
				}
			}
		}
	};
public:
	//!デフォルトの音量
	static constexpr float MAX_GAIN = 1.f;
	static constexpr float MIN_GAIN = 0.f;
	inline static Singleton& Get()
	{
		static auto inst = std::make_unique<Singleton>();
		return *inst;
	}
};

//!サウンド操作クラスです
class Sound final
{
private:
	std::string name_ = "";
	int handle_ = -1;
	float gain_ = 1.f;
public:
	Sound() = default;
	//!コンストラクタで登録したサウンドハンドル名を指定します
	Sound(const std::string& soundName)
	{
		assert(ResourceManager::GetSound().hasHandle(soundName));
		handle_ = ResourceManager::GetSound().getHandle(soundName);
		name_ = soundName;
	}
	//!登録したサウンドハンドル名を指定します
	void setSoundHandle(const std::string& soundName)
	{
		assert(ResourceManager::GetSound().hasHandle(soundName));
		handle_ = ResourceManager::GetSound().getHandle(soundName);
		name_ = soundName;
	}
	/**
	* @brief サウンドを再生します
	* @param  isLoop ループ再生するかどうか
	* @param  isContinuation stop()で止めたサウンドを続きから再生するかどうか。falseで続きから再生する
	* - 第二引数はデフォルトでtrueです。falseで再生すると再生位置が戻らないので連続で再生できません
	* - SEの時はtrueでBGMの時はfalseが適しています
	*/
	void play(const bool isLoop, bool isContinuation)
	{
		if (isLoop)
		{
			//trueなら最初から再生
			PlaySoundMem(handle_, DX_PLAYTYPE_LOOP, isContinuation);
		}
		else
		{
			PlaySoundMem(handle_, DX_PLAYTYPE_BACK, isContinuation);
		}
	}
	//!サウンドの再生を止めます
	void stop()
	{
		StopSoundMem(handle_);
	}
	//!サウンドが再生中か取得します
	//! - 停止した曲を途中から再生した場合、終了したのにfalseになるバグ?がある、ライブラリ由来のものか不明
	[[nodiscard]] const bool isPlay() const
	{
		switch (CheckSoundMem(handle_))
		{
		case 0: return false;
		case 1: return true;
		}
		return false;
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
};