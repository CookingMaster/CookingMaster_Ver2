/**
* @file Collision.hpp
* @brief Collisionの式をまとめたファイルです
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"

/**
* @brief Collisionの式をまとめたクラスです。
* メソッドはすべてstaticです
*/
class Collision
{
public:
	/**
	* @brief 矩形と矩形のあたり判定
	* @param b1 Entity
	* @param b2 Entity
	* @return bool
	* @details テンプレート引数にはIBoxColliderを継承したコンポーネントを指定してください
	* テンプレート引数に指定したコンポーネント同士で衝突判定を行います
	*/
	template<class T = ECS::BoxCollider, class T2 = ECS::BoxCollider>
	[[nodiscard]] inline static bool BoxAndBox(const ECS::Entity& b1, const ECS::Entity& b2)
	{
		if (!b1.hasComponent<T>() || !b2.hasComponent<T2>())
		{
			return false;
		}
		const auto& box1 = b1.getComponent<T>();
		const auto& box2 = b2.getComponent<T2>();
		if (box1.x() < box2.x() + box2.w() &&
			box2.x() < box1.x() + box1.w() &&
			box1.y() < box2.y() + box2.h() &&
			box2.y() < box1.y() + box1.h())
		{
			return true;
		}
		return false;
	}
	/**
	* @brief 矩形と矩形のあたり判定
	* @param b1Pos 矩形1の座標
	* @param b1Size 矩形1のサイズ
	* @param b2Pos 矩形2の座標
	* @param b2Size 矩形2のサイズ
	* @return bool
	*/
	[[nodiscard]] inline static bool BoxAndBox(const Vec2& b1Pos, const Vec2& b1Size, const Vec2& b2Pos, const Vec2& b2Size) noexcept
	{
		if (b1Pos.x < b2Pos.x + b2Size.x &&
			b2Pos.x < b1Pos.x + b1Size.x &&
			b1Pos.y < b2Pos.y + b2Size.y &&
			b2Pos.y < b1Pos.y + b1Size.y)
		{
			return true;
		}
		return false;
	}
	
	/**
	* @brief 円と円のあたり判定
	* @param e1 Entity
	* @param e2 Entity
	* @return bool
	* @details テンプレート引数にはICircleColliderを継承したコンポーネントを指定してください
	* テンプレート引数に指定したコンポーネント同士で衝突判定を行います
	*/
	template<class T = ECS::CircleCollider, class T2 = ECS::CircleCollider>
	[[nodiscard]] inline static bool CircleAndCircle(const ECS::Entity& e1, const ECS::Entity& e2)
	{
		if (!e1.hasComponent<T>() || !e2.hasComponent<T2>())
		{
			return false;
		}
		const auto& c1 = e1.getComponent<T>();
		const auto& c2 = e2.getComponent<T2>();
		if (((c1.x() - c2.x()) *(c1.x() - c2.x())) + ((c1.y() - c2.y()) * (c1.y() - c2.y())) <=
			(c1.radius() + c2.radius()) * (c1.radius() + c2.radius()))
		{
			return true;
		}
		return false;
	}

	/**
	* @brief 円と円のあたり判定
	* @param c1Pos 円1の座標
	* @param c1r 円1の半径
	* @param c2Pos 円2の座標
	* @param c2r 円2の半径
	* @return bool
	*/
	[[nodiscard]] inline static bool CircleAndCircle(const Vec2& c1Pos, const float& c1r, const Vec2& c2Pos, const float& c2r) noexcept
	{
		if (((c1Pos.x - c2Pos.x) *(c1Pos.x - c2Pos.x)) + ((c1Pos.y - c2Pos.y) * (c1Pos.y - c2Pos.y)) <=
			(c1r + c2r) * (c1r + c2r))
		{
			return true;
		}
		return false;
	}

	/**
	* @brief 円と点の当たり判定
	* @param e1 Entity
	* @param e2 Entity
	* @details テンプレート第一引数にはICircleColliderを継承したコンポーネントを指定してください
	* テンプレート引数に指定したコンポーネント同士で衝突判定を行います
	* @return bool
	*/
	template<class T = ECS::CircleCollider, class T2 = ECS::Position>
	[[nodiscard]] inline static bool CircleAndPoint(const ECS::Entity& e1, const ECS::Entity& e2)
	{
		if (!e1.hasComponent<T>() || !e2.hasComponent<T2>())
		{
			return false;
		}
		const auto& circle = e1.getComponent<T>();
		const auto& point = e2.getComponent<T2>();

		Vec2 buttonPos = Vec2(circle.x(), circle.y());
		Vec2 distance = buttonPos - point.val();
		if (distance.Length() <= circle.radius())
		{
			return true;
		}
		return false;
	}

	/**
	* @brief 円と点の当たり判定
	* @param circlePos 円の座標
	* @param circleRadius 円の半径
	* @param pointPos 点の座標
	* @return bool
	*/
	[[nodiscard]] inline static bool CircleAndPoint(const Vec2& circlePos, const float& circleRadius, const Vec2& pointPos) noexcept
	{
		Vec2 distance = circlePos - pointPos;
		if (distance.length() <= circleRadius)
		{
			return true;
		}
		return false;
	}
};
