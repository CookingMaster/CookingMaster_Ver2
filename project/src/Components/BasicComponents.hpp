/**
* @file BasicComponents.hpp
* @brief 座標や速度などの基本的なコンポーネント群です。
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include <DxLib.h>
#include <functional>
namespace ECS
{
	/*! 
	@brief  座標です,データの型はVec2です
	*/
	struct Position final : public ComponentData
	{
		Vec2 val;
		Position() = default;
		explicit Position(const Vec2& v) :val(v) {}
		explicit Position(const float& x, const float& y) :val(x, y) {}

	};
	/*!
	@brief  回転値です,データの型はfloatです
	*/
	struct Rotation final : public ComponentData
	{
		float val;
		Rotation() = default;
		Rotation(const float& r) : val(r) {}
		
	};
	/*!
	@brief  x,y方向の拡大率です,データの型はVec2です
	*/
	struct Scale final : public ComponentData
	{
		Vec2 val;
		Scale() = default;
		explicit Scale(const Vec2& scale) : val(scale) {}
		explicit Scale(const float& scale) : val(scale, scale) {}
		explicit Scale(const float& scaleX, const float& scaleY ) : val(scaleX, scaleY) {}
	};
	/*!
	@brief  速度です,データの型はVec2です
	*/
	struct Velocity final : public ComponentData
	{
		Vec2 val;
		Velocity() = default;
		explicit Velocity(const Vec2& v) :val(v) {}
		explicit Velocity(const float& x, const float& y) : val(x, y) {}
		
	};
	/*!
	@brief  向きです,データの型はenum class Dirです
	*/
	struct Direction final : public ComponentData
	{
		enum class Dir : short
		{
			R,
			L,
			U,
			D
		};
		Dir val;
		explicit Direction() : val(Dir::R) {};
		
	};
	/*!
	@brief 重力です,データの型はfloatです
	*/
	struct Gravity final : public ComponentData
	{
		static constexpr float DefaultGravity = 9.8f / 60 / 60 * 32 * 3;
		float val;
		Gravity() :val(DefaultGravity) {};
		explicit Gravity(const float g) :val(g) {}
	};

	/*
	@brief Entityに重力を加えます。
	また簡易的な衝突応答処理も含まれますが、これは明示的に呼び出してください
	@details Gravity, Velocity, Positionが必要です。衝突応答を行う場合はColliderが必要です
	現状だと1つのグループとの衝突応答しかできないのでこれを別のコンポーネントにするかもしれない
	*/
	class Physics final : public ComponentSystem
	{
	private:
		
		Gravity* gravity_;
		Velocity* velocity_;
		Position* pos_;
		std::vector<Entity*> otherEntity_;
		std::function<bool(const Entity&, const Entity&)> hitFunc_;
		void checkMove(Vec2& pos, Vec2& velocity)
		{
			Vec2 p(velocity);
			//横軸に対する移動
			while (p.x != 0.f)
			{
				float preX = pos.x;

				if (p.x >= 1)
				{
					pos.x += 1; p.x -= 1;
				}
				else if (p.x <= -1)
				{
					pos.x -= 1; p.x += 1;
				}
				else
				{
					pos.x += p.x;
					p.x = 0;
				}
				for (const auto& it : otherEntity_)
				{
					if (hitFunc_(*entity, *it))
					{
						velocity_->val.x = 0;
						pos.x = preX;		//移動をキャンセル
						break;
					}
				}
			
			}
			//縦軸に対する移動
			while (p.y != 0.f)
			{
				float preY = pos.y;
				if (p.y >= 1)
				{ 
					pos.y += 1; p.y -= 1; 
				}
				else if (p.y <= -1)
				{ 
					pos.y -= 1; p.y += 1; 
				}
				else
				{
					pos.y += p.y;
					p.y = 0; 
				}
				for (const auto& it : otherEntity_)
				{
					if (hitFunc_(*entity, *it))
					{
						velocity_->val.y = 0;
						pos.y = preY;		//移動をキャンセル
						break;
					}
				}
			}
		}
	public:
		void initialize() override
		{
			if (!entity->hasComponent<Gravity>())
			{
				entity->addComponent<Gravity>();
			}
			if (!entity->hasComponent<Velocity>())
			{
				entity->addComponent<Velocity>();
			}
			velocity_ = &entity->getComponent<Velocity>();
			gravity_ = &entity->getComponent<Gravity>();
			pos_ = &entity->getComponent<Position>();
		}
		void update() override 
		{
			velocity_->val.y += gravity_->val;
			checkMove(pos_->val, velocity_->val);
		}
		void setVelocity(const float& x, const float& y)
		{
			velocity_->val.x = x;
			velocity_->val.y = y;
		}
		void setGravity(const float& g = Gravity::DefaultGravity)
		{
			gravity_->val = g;
		}
		//!あたり判定の関数をセットする
		void setCollisionFunction(std::function<bool(const Entity& , const Entity& )> func)
		{
			hitFunc_ = func;
		}
		//!引数に指定したEntityにめり込まないようにする
		void pushOutEntity(std::vector<Entity*>&  e)
		{
			otherEntity_ = e;
		}
	};

	/*!
	@brief PositionとRotationとScaleをまとめます
	*/
	class Transform final : public ComponentSystem
	{
	private:
		Position* pos_;
		Rotation* rota_;
		Scale* scale_;
	public:
		Transform() :pos_(nullptr), rota_(nullptr) {}
		void initialize() override
		{
			if (!entity->hasComponent<Position>())
			{
				entity->addComponent<Position>(0.f, 0.f);
			}
			if (!entity->hasComponent<Rotation>())
			{
				entity->addComponent<Rotation>(0.f);
			}
			if (!entity->hasComponent<Scale>())
			{
				entity->addComponent<Scale>(1.f);
			}
			pos_ = &entity->getComponent<Position>();
			rota_ = &entity->getComponent<Rotation>();
			scale_ = &entity->getComponent<Scale>();
		}
		void setPosition(const float& x, const float& y)
		{
			pos_->val.x = x;
			pos_->val.y = y;
		}
		void setRotation(const float& r)
		{
			rota_->val = r;
		}
		void setScale(const float& scaleX, const float& scaleY)
		{
			scale_->val.x = scaleX;
			scale_->val.y = scaleY;
		}
	};

	/*!
	@brief コンストラクタで指定したフレーム後にEntityを殺します
	*/
	class KillEntity final : public ComponentSystem
	{
	private:
		int cnt_;
	public:
		KillEntity(const int span) :cnt_(span) {}

		void update() override
		{
			--cnt_;
			if (cnt_ <= 0)
			{
				entity->destroy();
			}
		}
		void kill()
		{
			cnt_ = 0;
		}
		void setKillLimit(const int limit)
		{
			cnt_ = limit;
		}
	};
}