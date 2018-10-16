/**
* @file Parameter.hpp
* @brief Scene間で渡すことのできるParameterクラスです
* @author tonarinohito
* @date 2018/10/14
*/
#pragma once
#include <unordered_map>
#include <any>

//!パラメータを自由に扱えるクラスです
class Parameter final
{
public:
	Parameter() = default;
	/*!
	* @brief Parameterコンテナに要素の追加をします。
	* - テンプレート引数に挿入したい型を指定してください
	* @param key キー
	* @param value 挿入したい値
	*/
	template<typename ValueType>
	void add(const std::string& key, ValueType value)
	{
		map[key] = value;
	}
	/*!
	* @brief 指定したキーの値を取得します
	* - テンプレート引数に取得したい型を指定してください
	*/
	template<typename ValueType>
	 [[nodiscard]] const ValueType get(const std::string& key) const
	{
		auto it = map.find(key);
		assert(map.end() != it && "キーのパラメータが存在しません");
		
		return std::any_cast<ValueType>(it->second);
	}
private:
	std::unordered_map<std::string, std::any> map;
};

