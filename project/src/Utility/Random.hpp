/**
 * @file Random.hpp
 * @brief 乱数に関する処理を記述します
 * @author tonarinohito
 * @date 2018/10/05
 */
#pragma once
#include <random>
/**
* @brief ランダムな値を扱います
*/
class Random
{
private:
	std::mt19937 mt_;

public:
	Random()
	{
		std::random_device device;
		mt_.seed(device());
	}

	//![min, max]範囲の一様乱数を取得します
	[[nodiscard]] const int getRand(int min, int max)
	{
		std::uniform_int_distribution<int> range(min, max);
		return range(mt_);
	}
	//![min, max]範囲の一様乱数を取得します
	[[nodiscard]] const float getRand(float min, float max)
	{
		std::uniform_real_distribution<float> range(min, max);
		return range(mt_);
	}
	//![min, max]範囲の一様乱数を取得します
	[[nodiscard]] const double getRand(double min, double max)
	{
		std::uniform_real_distribution<double> range(min, max);
		return range(mt_);
	}
};
