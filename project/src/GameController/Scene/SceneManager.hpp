/**
* @file SceneManager.hpp
* @brief Sceneオブジェクトを管理します	
* @author tonarinohito
* @date 2018/10/06
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Utility.hpp"
#include "Title.h"
#include "Game.h"

namespace Scene
{
	//!シーンの管理を行います
	class SceneManager final
	{
	public:
		//!シーンの状態です
		enum class State : short
		{
			TITLE,
			SELECT,
			PAUSE,
			GAME,
			RESULT
		};
	private:
		class Singleton final
		{
		private:
			State state_;
			IScene* pScene_ = nullptr;
		public:
			~Singleton()
			{
				Utility::SafeDelete(pScene_);
			}
			/**
			* @brief 処理したいシーンを決定します
			* @param scene 指定したいシーン
			* @param entityManager EntityManager
			*/
			void changeScene(const State& scene, ECS::EntityManager& entityManager)
			{
				Utility::SafeRelease(pScene_);
				Utility::SafeDelete(pScene_);
				switch (scene)
				{
				case State::TITLE:  pScene_ = new Title(entityManager); break;
				case State::SELECT:	break;
				case State::PAUSE:  break;
				case State::GAME:   pScene_ = new Game(entityManager); break;
				case State::RESULT: break;
				}
				state_ = scene;
			}
			//!現在指定されているシーンの更新を行います
			void update()
			{
				pScene_->update();
			}
			//!現在指定されているシーンの描画を行います
			void draw()
			{
				pScene_->draw();
			}
			//!現在のシーンを返します
			const State& getCurrentScene() const
			{
				return state_;
			}
		};
	public:
		static Singleton& Get()
		{
			static std::unique_ptr<Singleton> inst =
				std::make_unique<Singleton>();
			return *inst;
		}
	};
}