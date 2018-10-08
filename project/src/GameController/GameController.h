/**
 * @file GameController.hpp
 * @brief アプリケーションの処理を行います
 * @author tonarinohito
 * @date 2018/10/06
 */
#pragma once
#include "../ECS/ECS.hpp"
#define ENTITY_GROUP (ECS::Group)GameController::GameGroup
class GameController final
{
private:
	ECS::EntityManager entityManager_;
	void resourceLoad();
public:

	/**
	*  @brief エンティティ属するのグループです
	* - 描画レイヤー、処理順、エンティティの取得に使います
	* - OrderByDraw使用時、番号が大きいほど手前に描画されます
	*/
	enum class GameGroup : ECS::Group
	{
		LAYER0,
		LAYER1,
		MAX,
	};
	GameController();
	//!EntityおよびEventの更新処理を行います
	void update();
	//!Entityの描画を行います
	void draw();
};