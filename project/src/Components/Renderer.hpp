/**
* @file  Renderer.hpp
* @brief 描画関連のコンポーネントです。
* @note  特に理由がなければSpriteDrawやSpriteAnimationDraw、SpriteRectDrawが最も多機能なのでそれらの使用を推奨します
* @author tonarinohito
* @date 2018/10/06
* @par History
- 2018/10/12 tonarinohito
-# SpriteRectDraw追加
- 2018/10/25 tonarinohito
-# SpriteDrawもsetPivot()追加
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include "../Class/ResourceManager.hpp"
#include "../System/System.hpp"
#include <DxLib.h>
#include <DirectXMath.h>

namespace ECS
{
	/*!
	@brief 四角形のデータベース
	*/
	struct Rectangle final : public ComponentData
	{
	private:

	public:
		int x;
		int y;
		int w;
		int h;
		Rectangle():
			x(0),
			y(0),
			w(0),
			h(0)
		{}
		Rectangle(const int setX, const int setY, const int setW, const int setH) :
			x(setX),
			y(setY),
			w(setW),
			h(setH)
			{}

	};

	/*!
	@brief 色のデータです
	*RGBを扱います。データの型はintです
	*/
	struct Color final : public ComponentData
	{
		int red, green, blue;

		Color() :
			red(255),
			green(255),
			blue(255)
		{}
		Color(const int r, const int g, const int b) :
			red(r),
			green(g),
			blue(b)
		{}
	};

	/*!
	@brief アルファブレンドに関するデータです
	*ブレンドモードとアルファ値を扱います。データの型はenum BlendStateとintです
	*/
	struct AlphaBlend final : public ComponentData
	{
		//!DXライブラリのブレンドモードを簡潔にまとめたものです
		enum BlendMode
		{
			NOBLEND    = DX_BLENDMODE_NOBLEND,		//ノーブレンド（デフォルト）
			ALPHA      = DX_BLENDMODE_ALPHA,		//ブレンド
			ADD        = DX_BLENDMODE_ADD,			//加算ブレンド
			HALF_ADD   = DX_BLENDMODE_HALF_ADD,		//半加算ブレンド
			SUB        = DX_BLENDMODE_SUB,			//減算ブレンド
			MULA	   = DX_BLENDMODE_MULA,			//乗算ブレンド
			INVSRC     = DX_BLENDMODE_INVSRC,		//反転ブレンド
			PMA_ALPHA  = DX_BLENDMODE_PMA_ALPHA,	//乗算済みα用のαブレンド
			PMA_ADD    = DX_BLENDMODE_PMA_ADD,		//乗算済みα用の加算ブレンド
			PMA_SUB    = DX_BLENDMODE_PMA_SUB,		//乗算済みα用の減算ブレンド
			PMA_INVSRC = DX_BLENDMODE_PMA_INVSRC,   //乗算済みα用の反転ブレンド
		};
		BlendMode blendMode;
		int alpha;
		AlphaBlend() :
			blendMode(ALPHA),
			alpha(255)
		{}
		AlphaBlend(const BlendMode& brend, const int alpha) :
			blendMode(brend),
			alpha(alpha)
		{}
	};

	//!描画系の処理に共通する処理をまとめたものです
	struct RenderUtility final
	{
		//!色を設定します
		static void SetColor(const Color* color)
		{
			if (color != nullptr)
			{
				//RGBの設定
				SetDrawBright(color->red, color->green, color->blue);
			}
		}
		//!アルファブレンドを設定します
		static void SetBlend(const AlphaBlend* blend)
		{
			if (blend != nullptr)
			{
				//ブレンドの設定
				SetDrawBlendMode(blend->blendMode, blend->alpha);
			}
		}
		//!描画の状態をもとに戻します。必ず描画終了時に呼び出してください
		static void ResetRenderState()
		{
			//変更した色の情報を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			SetDrawBright(255, 255, 255);
		}
		//!エンティティにColorとAlphaBlendを安全に参照させます。
		static void SetRenderDetail(const Entity* entity, Color** color, AlphaBlend** blend)
		{
			//色データがあれば反映する
			if (entity->hasComponent<Color>())
			{
				*color = &entity->getComponent<Color>();
			}
			//ブレンド用データがあれば反映する
			if (entity->hasComponent<AlphaBlend>())
			{
				*blend = &entity->getComponent<AlphaBlend>();
			}
		}
	};
	

	/*!
	@brief 多機能な描画機能です。画像の中心が基準です
	* - Transfromが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	* - doCenterで基準座標を変更できます
	*/
	class SpriteDraw  : public ComponentSystem
	{
	private:
		bool isDiv_ = false;
	protected:
		Position* pos_ = nullptr;
		Scale* scale_ = nullptr;
		Rotation* rota_ = nullptr;
		Color* color_ = nullptr;
		AlphaBlend* blend_ = nullptr;
		std::string name_;
		Vec2_i size_;
		bool isDraw_ = true;
		bool isTurn = false;
		Vec2 pivot_;
	public:
		//!登録した画像名を指定して初期化します
		SpriteDraw(const char* name)
		{
			assert(ResourceManager::GetGraph().hasHandle(name) ||
				ResourceManager::GetGraph().hasDivHandle(name) && "load failed");
			if (ResourceManager::GetGraph().hasDivHandle(name))
			{
				isDiv_ = true;
			}
			name_ = name;
		}
		void initialize() override
		{
			pos_ = &entity->getComponent<Position>();
			rota_ = &entity->getComponent<Rotation>();
			scale_ = &entity->getComponent<Scale>();
			if (isDiv_)
			{
				GetGraphSize(ResourceManager::GetGraph().getDivHandle(name_,0), &size_.x, &size_.y);
			}
			else
			{
				GetGraphSize(ResourceManager::GetGraph().getHandle(name_), &size_.x, &size_.y);
			}
			pivot_.x = float(size_.x) / 2.f;
			pivot_.y = float(size_.y) / 2.f;
			RenderUtility::SetRenderDetail(entity, &color_, &blend_);
		}
		void draw2D() override
		{
			if (ResourceManager::GetGraph().hasHandle(name_) &&
				isDraw_)
			{
				RenderUtility::SetColor(color_);
				RenderUtility::SetBlend(blend_);
				DrawRotaGraph3F(
					pos_->val.x,
					pos_->val.y,
					pivot_.x,
					pivot_.y,
					scale_->val.x,
					scale_->val.y,
					DirectX::XMConvertToRadians(rota_->val),
					ResourceManager::GetGraph().getHandle(name_), true, isTurn);
				RenderUtility::ResetRenderState();
			}

		}
		//!描画を有効にします
		void drawEnable()
		{
			isDraw_ = true;
		}
		//!描画を無効にします
		void drawDisable()
		{
			isDraw_ = false;
		}
		//!描画する基準座標を引数で指定します
		void setPivot(const Vec2& pivot)
		{
			pivot_ = pivot;
		}
		//!画像サイズを返します
		const Vec2_i& getSize() const
		{
			return size_;
		}
		//!画像を反転させます
		void turnGraph()
		{
			isTurn = !isTurn;
		}

	};

	/*!
	@brief 多機能な分割画像描画機能です。画像の左上が基準です
	* - Transfromが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	* - setPivotで基準座標を変更できます
	*/
	class SpriteAnimationDraw final : public SpriteDraw
	{
	private:
		int index_ = 0;
	public:
		//!登録した画像名を指定して初期化します
		SpriteAnimationDraw(const char* name):
			SpriteDraw(name)
		{}
	
		void draw2D() override
		{
			if (ResourceManager::GetGraph().hasDivHandle(__super::name_) &&
				__super::isDraw_)
			{
				RenderUtility::SetColor(__super::color_);
				RenderUtility::SetBlend(__super::blend_);
				DrawRotaGraph3F(
					__super::pos_->val.x,
					__super::pos_->val.y,
					__super::pivot_.x,
					__super::pivot_.y,
					__super::scale_->val.x,
					__super::scale_->val.y,
					DirectX::XMConvertToRadians(__super::rota_->val),
					ResourceManager::GetGraph().getDivHandle(__super::name_,index_), true, isTurn);
				RenderUtility::ResetRenderState();
			}

		}
		//!描画したい分割画像の要素番号を指定します
		void setIndex(const int index)
		{
			index_ = index;
		}
		//!現在のインデックス値を取得する
		int getIndex()
		{
			return index_;
		}
	};

	/*!
	@brief 多機能な指定した範囲を描画する機能です。左上基準です
	* - Transfromが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	* - Rectangleが必要です。
	* - setPivotで基準座標を変更できます
	*/
	class SpriteRectDraw final : public SpriteDraw
	{
	private:
		Rectangle* rect_ = nullptr;
	public:
		//!登録した画像名を指定して初期化します
		SpriteRectDraw(const char* name):
			SpriteDraw(name)
		{}
		void initialize() override
		{
			__super::pos_ = &entity->getComponent<Position>();
			__super::rota_ = &entity->getComponent<Rotation>();
			__super::scale_ = &entity->getComponent<Scale>();
			rect_ = &entity->getComponent<Rectangle>();
			RenderUtility::SetRenderDetail(entity, &color_, &blend_);
		}
		void draw2D() override
		{
			if (ResourceManager::GetGraph().hasHandle(name_) &&
				isDraw_)
			{
				RenderUtility::SetColor(color_);
				RenderUtility::SetBlend(blend_);
				DrawRectRotaGraph3F(
					__super::pos_->val.x,
					__super::pos_->val.y,
					rect_->x,
					rect_->y,
					rect_->w,
					rect_->h,
					__super::pivot_.x,
					__super::pivot_.y,
					__super::scale_->val.x,
					__super::scale_->val.y,
					DirectX::XMConvertToRadians(rota_->val),
					ResourceManager::GetGraph().getHandle(name_),
					true);
				RenderUtility::ResetRenderState();
			}
		}
		//!描画する範囲を再設定します
		void setRect(const int srcX, const int srcY, const int w, const int h)
		{
			rect_->x = srcX;
			rect_->y = srcY;
			rect_->w = w;
			rect_->h = h;
		}
	};
}
