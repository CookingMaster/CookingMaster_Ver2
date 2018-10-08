/**
* @file Vec.hpp
* @brief ベクトルを扱うクラスです
* @author tonarinohito
* @date 2018/5/11
* @par History
- 2018/08/20 tonarinohito
-# 適切にconstをつけた
- 2018/10/04 tonarinohito
-# 静的でないメソッド名をキャメルケースに変更
- 2018/10/07 tonarinohito
-# TVecからVecTにリネーム
-# テンプレートコンストラクタ追加
*/
#pragma once

/**
*   @brief 2次元ベクトルを扱います
*/
template <class T> class Vec2T
{
public:
	T x, y;
	Vec2T() :
		x(0),
		y(0)
	{};

	template<class U>
	Vec2T(const Vec2T<U>& v)
	{
		x = v.x;
		y = v.y;
	}
	Vec2T(const T setX, const T setY)
	{
		x = setX;
		y = setY;
	};
	Vec2T(const Vec2T& v)
	{
		x = v.x;
		y = v.y;
	}
	~Vec2T() {};
	/*!
	* @brief オフセット値を返します
	* @return Vec2
	*/
	[[nodiscard]] const Vec2T offSetCopy(const T setX, const T setY) const
	{
		Vec2T ret(*this);
		ret.x += setX;
		ret.y += setY;
		return ret;
	}
	//! @brief すべてのパラメーターを0にします
	void zero()
	{
		x = 0;
		y = 0;
	}

	/*!
	* @brief 内積を返します
	* @return 内積
	*/
	[[nodiscard]] const T dot(const Vec2T& v) const
	{
		return x * v.x + y * v.y;
	}

	/*!
	* @brief 外積を返します
	* @return 外積
	*/
	[[nodiscard]] const T cross(const Vec2T& v) const
	{
		return x * v.y - v.x * y;
	}

	/*!
	* @brief 内積を返します
	* @return 内積
	*/
	[[nodiscard]] static T Dot(const Vec2T& v1, const Vec2T& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	/*!
	* @brief 外積を返します
	* @return 外積
	*/
	[[nodiscard]] static T Cross(const Vec2T& v1, const Vec2T& v2)
	{
		return v1.x * v2.y - v2.x * v1.y;
	}
	/*!
	* @brief 2点間の距離を返します
	* @return 距離
	*/
	[[nodiscard]] const T getDistance(const Vec2T& v) const
	{
		const T dx = x - v.x;
		const T dy = y - v.y;

		return hypot(dx, dy);
	}

	/*!
	* @brief 長さを返します
	* @return 長さ
	*/
	[[nodiscard]] const T length() const
	{
		return sqrtf(x * x + y * y);
	}
	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec2
	*/
	[[nodiscard]] const Vec2T&  normalize()
	{
		T tmp = length();
		x /= tmp;
		y /= tmp;
		return *this;
	}
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec2
	*/
	[[nodiscard]] const Vec2T&  getNormalize() const
	{
		Vec2T result = *this;
		result.normalize();
		return result;
	}

	Vec2T operator+() const
	{
		return Vec2T(*this);
	}

	Vec2T operator-() const
	{
		return Vec2T(-x, -y);
	}

	Vec2T operator+(const Vec2T& v) const
	{
		Vec2T ret(*this);
		ret += v;
		return ret;
	}

	Vec2T operator+(const T& t)
	{
		Vec2T ret(*this);
		ret += t;
		return ret;
	}

	Vec2T operator-(const Vec2T& v) const
	{
		Vec2T ret(*this);
		ret -= v;
		return ret;
	}

	Vec2T operator-(const T& t)
	{
		Vec2T ret(*this);
		ret -= t;
		return ret;
	}

	Vec2T operator*(const Vec2T& v)
	{
		Vec2T ret(*this);
		ret *= v;
		return ret;
	}

	Vec2T operator*(const T& t)
	{
		Vec2T ret(*this);
		ret *= t;
		return ret;
	}

	Vec2T operator/(const Vec2T& v)
	{
		Vec2T ret(*this);
		ret /= v;
		return ret;
	}

	Vec2T operator/(const T& t)
	{
		Vec2T ret(*this);
		ret /= t;
		return ret;
	}

	Vec2T& operator=(const Vec2T& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	Vec2T& operator=(const T& v)
	{
		x = v;
		y = v;
		return *this;
	}

	Vec2T& operator+=(const Vec2T& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2T& operator+=(const T& v)
	{
		x += v;
		y += v;
		return *this;
	}

	Vec2T& operator-=(const Vec2T& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2T& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		return *this;
	}

	Vec2T& operator*=(const Vec2T& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vec2T& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		return *this;
	}

	Vec2T& operator/=(const Vec2T& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vec2T& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		return *this;
	}

	[[nodiscard]] const T operator()(const int index) const
	{
		if (index == 0) return x;
		if (index == 1) return y;

	}

	bool operator==(const Vec2T &v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const Vec2T &v) const
	{
		return !(*this == v);
	}

};
typedef Vec2T<int>Vec2_i;
typedef Vec2T<float>Vec2;
typedef Vec2T<double>Vec2_d;


/**
*   @brief 3次元ベクトルを扱います
*/
template <class T> class Vec3T
{
public:
	T x, y, z;
	Vec3T() :
		x(0),
		y(0),
		z(0)
	{}
	template<class U>
	Vec3T(const Vec3T<U>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vec3T(const T setX, const T setY, const T setZ)
	{
		x = setX;
		y = setY;
		z = setZ;
	}
	Vec3T(const Vec3T& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	
	~Vec3T() {};
	/*!
	* @brief オフセット値を返します
	* @return Vec3
	*/
	[[nodiscard]] const Vec3T offSetCopy(const T setX, const T setY, const T setZ) const
	{
		Vec3T ret(*this);
		ret.x += setX;
		ret.y += setY;
		ret.z += setZ;
		return ret;
	}
	/*!
	* @brief 内積を返します
	* @return float
	*/
	[[nodiscard]] const T dot(const Vec3T& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	/*!
	* @brief 外積を返します
	* @return Vec3
	*/
	[[nodiscard]] const Vec3T cross(const Vec3T& v) const
	{
		Vec3T result;
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		return result;
	}
	/*!
	* @brief 内積を返します
	* @return 内積
	*/
	[[nodiscard]] static T Dot(const Vec3T& v1, const Vec3T& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	/*!
	* @brief 外積を返します
	* @return 外積
	*/
	[[nodiscard]] static Vec3T Cross(const Vec3T& v1, const Vec3T& v2)
	{
		Vec3T result;
		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;
		return result;
	}
	//! @brief すべてのパラメーターを0にします
	void zero()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	/*!
	* @brief 長さを返します
	* @return float
	*/
	[[nodiscard]] const T length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	/*!
	* @brief 自分自身を正規化した値を返します
	* @return Vec3
	*/
	[[nodiscard]] const Vec3T& normalize()
	{
		T tmp = length();
		x /= tmp;
		y /= tmp;
		z /= tmp;
		return *this;
	}
	/*!
	* @brief 自分自身を正規化した値のコピーを返します。自身の値は変わりません
	* @return Vec3
	*/
	[[nodiscard]] const Vec3T& getNormalize() const
	{
		Vec3T result = *this;
		result.normalize();
		return result;
	}
	/*!
	* @brief 2点間の距離を返します
	* @note C++17でないとエラー
	* @return 距離
	*/
	[[nodiscard]] const T getDistance(const Vec3T& v) const
	{
		const T dx = x - v.x;
		const T dy = y - v.y;
		const T dz = z - v.z;
		return std::hypot(dx, dy, dz);
	}

	Vec3T& operator=(const  Vec3T& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vec3T& operator=(const  T& v)
	{
		x = v;
		y = v;
		z = v;
		return *this;
	}

	Vec3T operator+() const
	{
		return Vec3T(*this);
	}

	Vec3T operator-() const
	{
		return Vec3T(-this->x, -this->y, -this->z);
	}

	Vec3T operator+(const Vec3T& v) const
	{
		Vec3T ret(*this);
		ret += v;
		return ret;
	}

	Vec3T operator+(const T& t)
	{
		Vec3T ret(*this);
		ret += t;
		return ret;
	}

	Vec3T operator-(const Vec3T& v) const
	{
		Vec3T ret(*this);
		ret -= v;
		return ret;
	}

	Vec3T operator-(const T& t)
	{
		Vec3T ret(*this);
		ret -= t;
		return ret;
	}

	Vec3T operator*(const Vec3T& v)
	{
		Vec3T ret(*this);
		ret *= v;
		return ret;
	}

	const Vec3T operator*(const T& t) const
	{
		Vec3T ret(*this);
		ret *= t;
		return ret;
	}

	Vec3T operator/(const Vec3T& v)
	{
		Vec3T ret(*this);
		ret /= v;
		return ret;
	}

	Vec3T operator/(const T& t)
	{
		Vec3T ret(*this);
		ret /= t;
		return ret;
	}

	Vec3T& operator+=(const Vec3T& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3T& operator+=(const T& v)
	{
		x += v;
		y += v;
		z += v;
		return *this;
	}

	Vec3T& operator-=(const Vec3T& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3T& operator-=(const T& v)
	{
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}

	Vec3T& operator*=(const Vec3T& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vec3T& operator*=(const T& v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	Vec3T& operator/=(const Vec3T& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vec3T& operator/=(const T& v)
	{
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	[[nodiscard]] const T operator()(const int idx) const
	{
		if (idx == 0) return x;
		if (idx == 1) return y;
		if (idx == 2) return z;
	}

	bool operator==(const Vec3T &v) const
	{
		return (x == v.x && y == v.y && v.z == z);
	}
	bool operator!=(const Vec3T &v) const
	{
		return !(*this == v);
	}

};
typedef Vec3T<int>Vec3_i;
typedef Vec3T<float>Vec3;
typedef Vec3T<double>Vec3_d;
