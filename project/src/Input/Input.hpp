﻿/**
* @file Input.hpp
* @brief 入力関連の処理をまとめます
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include <memory>
#include <DxLib.h>
/*!
@brief キーイベントクラスです
*/
class Input final
{
private:
	class Singleton final
	{
	private:
		//キーの入力状態を格納する
		int key_[256];
		bool isAnyInput;
	public:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		/**
		* @brief キーの入力状態を更新します
		*/
		void updateKey()
		{
			char tmpKey[256];
			GetHitKeyStateAll(tmpKey);	//現在のキーの状態を格納
			isAnyInput = false;
			for (int i = 0; i < 256; ++i)
			{
				if (tmpKey[i] != 0)
				{
					++key_[i];
					isAnyInput = true;
				}
				else  //押されていなければ
				{
					key_[i] = 0;
					isAnyInput = isAnyInput || false;
				}
			}
		}
		/**
		* @brief キーの入力状態を取得します
		* @param keycode 調べたいキーコード
		* @return 押しているフレーム数。押されていない場合0が返ります
		*/
		[[nodiscard]] int getKeyFrame(int keycode)
		{
			return key_[keycode];	
		}

		/**
		* @brief いずれかのキーが入力されていたらtrueを返す
		* @return 押されているか否か
		*/
		[[nodiscard]] bool getIsAnyInput()
		{
			return isAnyInput;
		}
	};
public:
	inline static Singleton& Get()
	{
		static std::unique_ptr<Singleton> inst =
			std::make_unique<Singleton>();
		return *inst;
	}
};
