/**
* @file EventManager.hpp
* @brief イベント処理を管理するクラスです
* @author tonarinohito
* @date 2018/10/05
*/
#pragma once
#include <string>
#include <iostream>
#include <functional>
#include "../ECS/ECS.hpp"
#include "../GameController/Scene/SceneManager.hpp"
namespace Event
{
	//!シーンごとのイベント処理を管理するクラスです
	class EventManager final
	{
	private:
		class Singleton final
		{
		private:
			using SceneManager = Scene::SceneManager;
			std::vector<std::pair<SceneManager::State, std::function<void(ECS::EntityManager&)>>> events_;
		public:
			/**
			* @brief  イベント関数を追加します
			* @param  state 処理を行うシーン
			* @param  func イベント処理
			*/
			void addEvent(const SceneManager::State state, std::function<void(ECS::EntityManager&)> func)
			{
				events_.emplace_back(std::make_pair(state, func));
			}
			//!イベントを実行します
			void update(ECS::EntityManager& entityManager)
			{
				for (auto& it : events_)
				{
					if (SceneManager::Get().getCurrentScene() == it.first)
					{
						it.second(entityManager);
					}
				}
			}
		};
	public:
		static Singleton& Get()
		{
			static auto inst = std::make_unique<Singleton>();
			return *inst;
		}
	};
}
