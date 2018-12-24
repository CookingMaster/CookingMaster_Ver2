/**
* @file Counter.hpp
* @brief Counterを扱うクラスです
* @author tonarinohito
* @date 2018/10/05
* @par History
- 2018/10/12 tonarinohito
-# reset()修正
- 2018/10/16 tonarinohito
-# ++や--の属性を消した
*/
#pragma once

/*! @class Counter
*   @brief カウンターです.上限値に達したら下限値、下限値を超えたら上限値になります
*   @details コンストラクタで初期値、加算値、下限値、上限値を設定してください
*   上限値や下限値を設定していない場合、その型の最大値、最小値が設定されます
*   何も指定しない場合の加算値は1になります
*/
template <typename T> class TCounter
{
#undef max
#undef min
private:
	bool isMax_;
	bool isMin_;
	T now_;
	T add_;
	T min_;
	T max_;
public:
	
	TCounter() :
		isMax_(false),
		isMin_(false),
		now_(0),
		add_(1),
		min_(std::numeric_limits<T>::min()),
		max_(std::numeric_limits<T>::max())
	{};
	//!加算値のみ設定
	explicit TCounter(T add) :
		isMax_(false),
		isMin_(false),
		now_(0),
		add_(add),
		min_(std::numeric_limits<T>::min()),
		max_(std::numeric_limits<T>::max())
	{};
	//!下限値と上限値設定
	explicit TCounter(T min, T max) :
		isMax_(false),
		isMin_(false),
		now_(0),
		add_(1),
		min_(min),
		max_(max)
	{};
	//!すべてのパラメータ設定
	explicit TCounter(T now, T add, T min, T max) :
		isMax_(false),
		isMin_(false)
	{
		now_ = now;
		add_ = add;
		min_ = min;
		max_ = max;
	}
	/*!* @brief カウンターを初期化します*/
	void setCounter(T now, T add, T min, T max) 
	{
		now_ = now;
		add_ = add;
		min_ = min;
		max_ = max;
		isMax_ = false;
		isMin_ = false;
	}
	/*!
	* @brief カウンターを加算値分足します
	* @return 現在のカウンターの値
	*/
	const T operator++()
	{
		if (now_ >= max_)
		{
			now_ = min_;
		}
		else
		{
			now_ += add_;
		}
		return now_;
	}

	/*!
	* @brief カウンターを加算値分引きます
	* @return 現在のカウンターの値
	*/
	const T operator--()
	{
		if (now_ <= min_)
		{
			now_ = max_;
		}
		else
		{
			now_ -= add_;
		}
		return now_;
	}
	//!終了時間まで加算値分増やします。上限以上になったら停止します
	void add() 
	{
		if (now_ >= max_)
		{
			return;
		}
		now_ += add_;
	}
	//!終了時間まで加算値分引きます。下限以下になったら停止します
	void sub() 
	{
		if (now_ <= min_)
		{
			return;
		}
		now_ -= add_;
	}
	//!経過時間を0にし、フラグを再セットします
	void reset(const T& resetNow = 0) 
	{
		isMax_ = false;
		isMin_ = false;
		now_ = resetNow;
	}

	//!現在のカウンターの値をミリ秒で返します
	[[nodiscard]] const T getMilliSecond(T frameRate) const
	{
		return (now_ / frameRate) * 1000;
	}

	//!終了時間を設定します
	void setEndTime(T max, T min) 
	{
		max_ = max;
		min_ = min;
	}
	//!終了時間を設定します
	void setEndTime(T endTime)
	{
		max_ = endTime;
		min_ = -endTime;
	}
	//!終了時間を型の最大値にします
	void setInfinity()
	{
		max_ = std::numeric_limits<T>::max();
		min_ = std::numeric_limits<T>::min();
	}

	//!残り時間(終了時間-経過時間)を返す
	[[nodiscard]] const T getRemainingTime() const
	{
		return max_ - now_;
	}
	//!現在のカウンターの値を返します
	[[nodiscard]] const T& getCurrentCount() const
	{
		return now_;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも上限値以上に到達したか返します
	* @return bool
	*/
	[[nodiscard]] const bool isMax()
	{
		if (now_ >= max_)
		{
			isMax_ = true;
		}
		return isMax_;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも下限値以下に到達したか返します
	* @return bool
	*/
	[[nodiscard]] const bool isMin()
	{
		if (now_ <= min_)
		{
			isMin_ = true;
		}
		return isMin_;
	}
};

typedef TCounter<int>Counter;
typedef TCounter<float>Counter_f;
typedef TCounter<double>Counter_d;
typedef TCounter<long>Counter_l;
typedef TCounter<long long>Counter_ll;
