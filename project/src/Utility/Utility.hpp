/**
 * @file Utility.hpp
 * @brief 便利な処理群です
 * @author tonarinohito
 * @date 2018/10/04
 */
#pragma once
#include <iostream>
#include <chrono>
#include <cstdio>
#include <windows.h>

#ifdef _DEBUG
 //!デバッグ時にコンソールを表示します
#define ShowConsole() Utility::Console c
//!コンソールにcoutを許可します
#define StartOutputDbgString() Utility::coutDbgString c
/**
* @brief デバッグ時にコンソールへの出力をする命令です。
* @details 呼び出した関数名も出力されます
* ラムダ式では使えません。関数オブジェクトではoperator()となってしまいます。
*/
#define DOUT_BY_FUNCTION std::cout << __func__ << ":"
//!デバッグ時にコンソールへの出力をする命令です。
#define DOUT std::cout
/**
* @brief コンソールへの出力時にファイル名と行数を表示します
* @details DOUT << FILENAME_AND_LINE; のように使います
*/
#define FILENAME_AND_LINE __FILE__ << ":" << __LINE__
#else 
#define DOUT_BY_FUNCTION 0 && std::cout 
#define DOUT 0 && std::cout
#define FILENAME_AND_LINE 0 && std::cout
#define ShowConsole() __noop
#define StartOutputDbgString() __noop
#endif
namespace Utility
{
	/*!
	* @class Console
	* @brief Win32アプリでcin、coutを許可するクラスです
	*/
	class Console final
	{
		std::FILE* in_;
		std::FILE* out_;
	public:
		Console()
		{
			AllocConsole();
			freopen_s(&out_, "CONOUT$", "w", stdout); //標準出力をコンソールにする
			freopen_s(&in_, "CONIN$", "r", stdin);  //標準入力をコンソールにする
		}
		~Console()
		{
			std::fclose(in_);
			std::fclose(out_);
			FreeConsole();
		}
	};

	/*!
	@class coutDbgString
	@brief coutでデバッグウインドウに出力できるようになるクラスです
	*/
	class coutDbgString final
	{
	private:
		class DbgStreambuf final : public std::streambuf
		{
		public:
			[[nodiscard]] std::streamsize xsputn(const char* s, std::streamsize n)
			{
				OutputDebugString(s);
				return n;
			}
			[[nodiscard]] int_type overflow(int_type c = EOF)
			{
				if (c != EOF)
				{
					char buf[2] = { (char)c, '\0' };
					OutputDebugString(buf);
				}
				return c;
			}
		};
		std::streambuf* default_stream;
		DbgStreambuf debug_stream;
	public:
		coutDbgString()
		{
			default_stream = std::cout.rdbuf(&debug_stream);
		}
		~coutDbgString()
		{
			std::cout.rdbuf(default_stream);
		}
	};

	//!引数に入れたポインタを解放します
	template<class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	//!引数に入れた配列を解放します
	template<class T> void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}

	//!引数に入れたオブジェクトを解放します
	template<class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->release();
			t = nullptr;
		}
	}

	/*!
	* @class ProcessingTime
	* @brief 処理時間を計測します。
	* @details　インスタンス化されたら計測を開始し、デストラクタで計測結果をコンソールに書き出します
	*/
	template<class T = std::chrono::milliseconds>
	class ProcessingTime final
	{
	private:
		std::string name_;
		std::chrono::system_clock::time_point start_;
		bool isActive_;

	public:
		//!計測を開始します
		ProcessingTime(const std::string& a = "Process", bool start = true) :
			name_(a),
			isActive_(start)
		{
			if (start)
			{
				this->reStart();
			}
		}
		~ProcessingTime()
		{
			stop();
		}

		//!計測のリスタート
		void reStart()&
		{
			start_ = std::chrono::system_clock::now();
			isActive_ = true;
		}
		//!計測を終了し出力
		void stop()&
		{
			if (!isActive_)
				return;
			const auto end = std::chrono::system_clock::now();
			const auto elapsed = std::chrono::duration_cast<T>(end - start_).count();
			if (typeid(T) == typeid(std::chrono::nanoseconds))
			{
				DOUT << elapsed << " [nanoseconds]" << std::endl;
				return;
			}
			if (typeid(T) == typeid(std::chrono::microseconds))
			{
				DOUT << elapsed << " [microseconds]" << std::endl;
				return;
			}
			if (typeid(T) == typeid(std::chrono::milliseconds))
			{
				DOUT << elapsed << " [milliseconds]" << std::endl;
				return;
			}
			if (typeid(T) == typeid(std::chrono::seconds))
			{
				DOUT << elapsed << " [seconds]" << std::endl;
				return;
			}
			if (typeid(T) == typeid(std::chrono::minutes))
			{
				DOUT << elapsed << " [minutes]" << std::endl;
				return;
			}
			if (typeid(T) == typeid(std::chrono::hours))
			{
				DOUT << elapsed << " [hours]" << std::endl;
				return;
			}
			isActive_ = false;
		}
	};
}
