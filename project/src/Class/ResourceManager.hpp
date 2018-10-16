/**
* @file ResourceManager.hpp
* @brief DXライブラリのLoadGraphやLoadSoundMemから取得したハンドルを管理するクラスです
* @author tonarinohito
* @date 2018/10/05
* @par History
- 2018/10/14 tonarinohito
-# load系メソッドが戻り値を返すようにした
-# load系メソッドにて登録の重複がある場合、そのハンドルを返すようにした
*/
#pragma once
#include <DxLib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <cassert>
#include "../Utility/Utility.hpp"

//!サウンドの種類
enum class SoundType
{
	BGM,
	SE
};

//!グラフィックやサウンドのハンドル管理をします
class ResourceManager final
{
private:
	/*!
	@brief Graphicハンドルを管理します
	*/
	class GraphicManager final
	{
	private:
		typedef std::unordered_map<std::string, int> GraphMap;
		typedef std::unordered_map<std::string, std::pair<int*, size_t>> DivGraphMap;
		GraphMap graphs_;
		DivGraphMap divGraphs_;
	public:
		~GraphicManager()
		{
			for (auto& it : divGraphs_)
			{
				Utility::SafeDeleteArray(it.second.first);
			}
			InitGraph();
		}
		/**
		* @brief  画像をロードします
		* @param  path ファイルパス
		* @param  name 登録名
		* @detail 既に登録した名前は使えません
		* @return 登録したハンドルが返ります。
		* - すでに登録した名前を指定したらそのハンドルが返ります
		*/
		int load(const std::string& path, const std::string& name)
		{
			//名前の重複防止
			if (graphs_.count(name))
			{
				DOUT << "GraphicHandle :" + name + " load is failed" << std::endl;
				return graphs_[name];
			}
			graphs_[name] = LoadGraph(path.c_str());
			if (graphs_[name] == -1)
			{
				DOUT << path + " load is failed" << std::endl;
				assert(false && " load is failed");
			}
			return graphs_[name];
		}
		/**
		* @brief  画像を非同期でロードします
		* @param  path ファイルパス
		* @param  name 登録名
		* @detail 既に登録した名前は使えません。非同期なのでこのメソッドで処理が止まることはありません
		* @return 正常に読み込めたら1が返ります
		* - すでに登録した名前を指定したらそのハンドルが返ります
		*/
		int loadAsync(const std::string& path, const std::string& name)
		{
			//名前の重複防止
			if (graphs_.count(name))
			{
				DOUT << "GraphicHandle :" + name + " load is failed" << std::endl;
				return graphs_[name];
			}
			SetUseASyncLoadFlag(TRUE); // 非同期読み込みフラグON
			graphs_[name] = LoadGraph(path.c_str());
			if (graphs_[name] == -1)
			{
				DOUT << path + " load is failed" << std::endl;
				assert(false && " load is failed");
			}
			SetUseASyncLoadFlag(FALSE); // 非同期読み込みフラグOFF
			return 1;
		}
		/**
		* @brief  指定したハンドルの読み込みが完了しているか返します
		* @param  name 登録名
		* @return 読み込み済みならしたらture
		*/
		[[nodiscard]] bool isLoaded(const std::string& name)
		{
			switch (CheckHandleASyncLoad(graphs_[name]))
			{
			case -1:
				if (graphs_[name] == -1)
				{
					DOUT << name + " loadAsync is failed" << std::endl;
					assert(false && " loadAsync is failed");
				}
				break;

			case FALSE: return true;	//非同期読み込み済み
			case TRUE:  return false;	//まだ

			}
		}
		/**
		* @brief  指定したハンドルの読み込みが完了しているか返します
		* @param  name 登録名
		* @return 読み込み済みならしたらture
		*/
		[[nodiscard]] bool isLoadedDiv(const std::string& name)
		{
			switch (CheckHandleASyncLoad(*divGraphs_[name].first))
			{
			case -1:
				if (*divGraphs_[name].first == -1)
				{
					DOUT << name + " loadAsync is failed" << std::endl;
					assert(false && " loadAsync is failed");
				}
				break;

			case FALSE: return true;	//非同期読み込み済み
			case TRUE:  return false;	//まだ

			}
		}
		/**
		* @brief  分割画像をロードします
		* @param  path ファイルパス
		* @param  name 登録名
		* @param  allNum 画像の分割総数
		* @param  xNum 横方向の数
		* @param  yNum 縦方向の数
		* @param  xSize 分割した画像一枚分の横のサイズ
		* @param  ySize 分割した画像一枚分の縦のサイズ
		* @return 登録したハンドルの先頭要素が返ります。
		* - すでに登録した名前を指定したらそのハンドルの先頭要素が返ります
		*/
		int loadDiv(const std::string& path, const std::string& name,
			const int allNum,
			const int xNum, const int yNum,
			const int xSize, const int ySize)
		{
			//名前の重複防止
			if (divGraphs_.count(name))
			{
				DOUT << "GraphicHandle :" + name + " load is failed" << std::endl;
				return divGraphs_[name].first[0];
			}
			divGraphs_[name].first = new int[allNum];
			divGraphs_[name].second = (size_t)allNum;
			int isOk = LoadDivGraph(path.c_str(), allNum, xNum, yNum, xSize, ySize, divGraphs_[name].first);
			if (isOk == -1)
			{
				DOUT << path + " load is failed" << std::endl;
				assert(false && " load is failed");
			}
			return divGraphs_[name].first[0];
		}
		/**
		* @brief  分割画像を非同期でロードします
		* @param  path	ファイルパス
		* @param  name	登録名
		* @param  allNum 画像の分割総数
		* @param  xNum 横方向の数
		* @param  yNum 縦方向の数
		* @param  xSize 分割した画像一枚分の横のサイズ
		* @param  ySize 分割した画像一枚分の縦のサイズ
		* @return 正常に読み込めたら1が返ります
		* - すでに登録した名前を指定したらそのハンドルの先頭要素が返ります
		*/
		int loadDivAsync(const std::string& path, const std::string& name,
			const int allNum,
			const int xNum, const int yNum,
			const int xSize, const int ySize)
		{
			//名前の重複防止
			if (divGraphs_.count(name))
			{
				DOUT << "GraphicHandle :" + name + " load is failed" << std::endl;
				return divGraphs_[name].first[0];
			}
			SetUseASyncLoadFlag(TRUE); // 非同期読み込みフラグON
			divGraphs_[name].first = new int[allNum];
			divGraphs_[name].second = (size_t)allNum;
			int isOk = LoadDivGraph(path.c_str(), allNum, xNum, yNum, xSize, ySize, divGraphs_[name].first);
			if (isOk == -1)
			{
				DOUT << path + " loadAsync is failed" << std::endl;
				assert(false && " loadAsync is failed");
			}
			SetUseASyncLoadFlag(FALSE); // 非同期読み込みフラグOFF
			return 1;
		}
		/**
		* @brief  メモリに読み込んだ画像のハンドルを返します
		* @param  name 登録名
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするとエラーになります
		*/
		[[nodiscard]] int getHandle(const std::string& name)
		{
			if (graphs_.find(name) == graphs_.end())
			{
				DOUT << "Registered name :" + name + " is not found" << std::endl;
				assert(false);
			}
			return graphs_[name];
		}
		/**
		* @brief  メモリに読み込んだ分割画像のハンドルを返します
		* @param  name 登録名
		* @param  index 配列の要素数
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするか分割数を超えた値を指定するとエラーになります
		*/
		[[nodiscard]] int getDivHandle(const std::string& name, const int index)
		{
			if (divGraphs_.find(name) == divGraphs_.end())
			{
				DOUT << "Registered name :" + name + " is not found" << std::endl;
				assert(false);
			}
			if ((size_t)index >= divGraphs_[name].second)
			{
				DOUT << "Registered name :" + name + " is out of range" << std::endl;
				assert(false);
			}
			return divGraphs_[name].first[index];
		}
		/**
		* @brief  メモリに読み込んだ画像のハンドルが存在するか返します
		* @param  name 登録名
		* @return ハンドルが存在したらtrue
		*/
		[[nodiscard]] bool hasHandle(const std::string& name)
		{
			if (graphs_.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief  メモリに読み込んだ分割画像のハンドルが存在するか返します
		* @param  name 登録名
		* @return ハンドルが存在したらtrue
		*/
		[[nodiscard]] bool hasDivHandle(const std::string& name)
		{
			if (divGraphs_.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief  メモリに読み込んだ画像リソースを解放します
		* @param  name 登録名
		* @return 登録名が存在しない場合何も起きません
		*/
		void removeDivGraph(const std::string& name)
		{
			if (divGraphs_.find(name) == divGraphs_.end() || !divGraphs_[name].first)
			{
				DOUT << "Registered name :" + name + " is remove failed" << std::endl;
				return;
			}
			DeleteGraph(*divGraphs_[name].first);
			Utility::SafeDeleteArray(divGraphs_[name].first);
			divGraphs_.erase(name);
		}
		/**
		* @brief  メモリに読み込んだ分割画像リソースを解放します
		* @param  name 登録名
		* @return 登録名が存在しない場合何も起きません
		*/
		void removeGraph(const std::string& name)
		{
			if (graphs_.find(name) == graphs_.end() || !graphs_[name])
			{
				DOUT << "Registered name :" + name + " is remove failed" << std::endl;
				return;
			}
			DeleteGraph(graphs_[name]);
			graphs_.erase(name);
		}
	};

	/*!
	@class SoundManager
	@brief サウンドハンドルを管理します
	*/
	class SoundManager final
	{
	private:
		typedef std::unordered_map<std::string, std::pair<int, SoundType>> SoundMap;
		SoundMap sounds_;
	public:

		~SoundManager()
		{
			InitSoundMem();
		}
		/**
		* @brief サウンドをロードします
		* @param  path ファイルパス
		* @param  name 登録名
		* @param  soundType BGMかSEか列挙型で指定
		* @return 登録したハンドルが返ります。
		* - すでに登録した名前を指定したらそのハンドルが返ります
		*/
		int load(const std::string& path, const std::string& name, const SoundType& soundType)
		{
			//名前の重複防止
			if (sounds_.count(name))
			{
				DOUT << "SoundHandle :" + name + " load is failed" << std::endl;
				return sounds_[name].first;
			}
			sounds_[name].second = soundType;
			sounds_[name].first = LoadSoundMem(path.c_str());
			if (sounds_[name].first == -1)
			{
				DOUT << path + " load is failed" << std::endl;
				assert(false && " load is failed");
			}
			return sounds_[name].first;
		}
		/**
		* @brief  サウンドを非同期でロードします
		* @param  path ファイルパス
		* @param  name 登録名
		* @return 正常に読み込めたら1が返ります
		* - すでに登録した名前を指定したらそのハンドルが返ります
		*/
		int loadAsync(const std::string& path, const std::string& name, const SoundType& soundType)
		{
			//名前の重複防止
			if (sounds_.count(name))
			{
				DOUT << "SoundHandle :" + name + " load is failed" << std::endl;
				return sounds_[name].first;
			}
			sounds_[name].second = soundType;
			SetUseASyncLoadFlag(TRUE); // 非同期読み込みフラグON
			sounds_[name].first = LoadSoundMem(path.c_str());
			if (sounds_[name].first == -1)
			{
				DOUT << path + " load is failed" << std::endl;
				assert(false && " load is failed");
			}
			SetUseASyncLoadFlag(FALSE); // 非同期読み込みフラグOFF
			return 1;
		}
		/**
		* @brief  指定したハンドルの読み込みが完了しているか返します
		* @param  name 登録名
		* @return 読み込み済みならしたらture
		*/
		[[nodiscard]] bool isLoaded(const std::string& name)
		{
			switch (CheckHandleASyncLoad(sounds_[name].first))
			{
			case -1:
				if (sounds_[name].first == -1)
				{
					DOUT << name + " loadAsync is failed" << std::endl;
					assert(false && " loadAsync is failed");
				}
				break;

			case FALSE: return true;	//非同期読み込み済み
			case TRUE:  return false;	//まだ

			}
		}
		/**
		* @brief  メモリに読み込んだサウンドハンドルを返します
		* @param  name 登録名
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするとエラーになります
		*/
		[[nodiscard]] int getHandle(const std::string& name)
		{
			if (sounds_.find(name) == sounds_.end())
			{
				DOUT << "Registered name :" + name + " is not found" << std::endl;
				assert(false);
			}
			return sounds_[name].first;
		}
		/**
		* @brief メモリに読み込んだサウンドハンドルが存在するか返します
		* @param name 登録名
		* @return ハンドルが存在したらtrue
		*/
		[[nodiscard]] bool hasHandle(const std::string& name)
		{
			if (sounds_.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief メモリに読み込んだサウンドリソースを解放します
		* @param name 登録名
		* @return 登録名が存在しない場合何も起きません
		*/
		void remove(const std::string& name)
		{
			if (sounds_.find(name) == sounds_.end() || !sounds_[name].first)
			{
				DOUT << "Registered name :" + name + " is remove failed" << std::endl;
				return;
			}
			DeleteSoundMem(sounds_[name].first);
			sounds_.erase(name);
		}
		//!すべてのハンドルをunordered_mapで返します
		[[nodiscard]] const SoundMap& getSoundMap() const
		{
			return sounds_;
		}
	};


public:
	/**
	* @brief 非同期読み込み中の処理数を返します
	* @return 非同期処理中の数
	*/
	static int GetAsyncLoadNum()
	{
		return GetASyncLoadNum();
	}
	/** @brief GraphicManagerを取得します*/
	static GraphicManager& GetGraph()
	{
		static std::unique_ptr<GraphicManager> pGraph =
			std::make_unique<GraphicManager>();
		return *pGraph;
	}
	/** @brief SoundManagerを取得します*/
	static SoundManager& GetSound()
	{
		static std::unique_ptr<SoundManager> pSound =
			std::make_unique<SoundManager>();
		return *pSound;
	}
};