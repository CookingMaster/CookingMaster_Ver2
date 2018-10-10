/**
* @file  ECS.hpp
* @brief EntityComponentSystem
* @author tonarinohito
* @date 2018/10/05
* @note  参考元 https://github.com/SuperV1234/Tutorials
*/
#pragma once
#include <bitset>
#include <array>
#include <memory>
#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>

/**
* @brief EntityComponentSystemに関連した機能群
*/
namespace ECS
{
	class Entity;
	class ComponentSystem;
	class EntityManager;

	using ComponentID = std::size_t;
	using Group = std::size_t;

	//!AddされたらコンポーネントのIDをインクリメントする関数
	[[nodiscard]] inline ComponentID GetNewComponentTypeID() noexcept
	{
		static ComponentID lastID = 0;
		return ++lastID;
	}
	//!複数のコンポーネントをIDによって管理するための関数
	template <typename T> [[nodiscard]] inline ComponentID GetComponentTypeID() noexcept
	{
		static ComponentID typeID = GetNewComponentTypeID();
		return typeID;
	}

	//!最大コンポーネント数。必要に応じて限界値は変える
	constexpr std::size_t MaxComponents = 32;
	//!最大グループ数。必要に応じて限界値は変える
	constexpr std::size_t MaxGroups = 32;

	//!コンポーネントのフラグ管理用
	using ComponentBitSet = std::bitset<MaxComponents>;
	//!フラグとIDを関連付けるために静的配列で用意
	using ComponentArray = std::array<ComponentSystem*, MaxComponents>;
	//!Groupも同様にIDを関連付けるために静的配列で用意
	using GroupBitSet = std::bitset<MaxGroups>;

	/** 
    * @brief Componentの基底クラスです
	* @details Entityに対するすべての振る舞いはこのクラスを継承し実装します
	*/
	class ComponentSystem
	{
	private:
		//Entityによって殺されたいのでこうなった
		friend class Entity;
		bool active_ = true;
		void removeThis() { active_ = false;}
	public:
		Entity* entity;
		virtual void initialize() {};
		virtual void update() {};
		virtual void draw3D() {};
		virtual void draw2D() {};
		virtual ~ComponentSystem() {}
		//このコンポーネントが生きているか返します
		[[nodiscard]] virtual bool isActive() const final { return active_; }

	};

	/**
	* @brief ComponentDataの基底クラスです
	* @details データにはメソッドを持たせません。
	* 複数の振る舞いにおいて共通で使用したい値やパブリックな値が該当します
	*/
	struct ComponentData : public ComponentSystem
	{
		[[deprecated("can not use")]] void update() override final {}
		[[deprecated("can not use")]] void draw3D() override final {}
		[[deprecated("can not use")]] void draw2D() override final {}
	};

	/**
	* @brief 1つ以上のコンポーネントによって定義されるEntityです
	* @details データや振る舞い、グループを設定し使用してください
	*/
	class Entity final
	{
	private:
		friend class EntityManager;
		std::string tag_;
		EntityManager& manager_;
		bool isActive_ = true;
		std::vector<std::unique_ptr<ComponentSystem>> components_;
		ComponentArray  componentArray_;
		ComponentBitSet componentBitSet_;
		GroupBitSet groupBitSet_;
		//!非アクティブなコンポーネントを消す
		void refreshComponent()
		{
			components_.erase(std::remove_if(std::begin(components_), std::end(components_),
				[](const std::unique_ptr<ComponentSystem> &pCom)
			{
				return !pCom->isActive();
			}),
				std::end(components_));
		}
		
	public:
		//!コンストラクタでマネージャーを指定してください
		Entity(EntityManager& manager) : manager_(manager) {}
		//!このEntityについているComponentの初期化処理を行います
		void initialize()
		{
			for (auto& c : components_) c->initialize();
		}

		//!このEntityについているComponentの更新処理を行います
		void update()
		{
			refreshComponent();
			for (auto& c : components_)
			{
				if (c == nullptr)
				{
					continue;
				}
				c->update();
			}
		}

		//!このEntityについているComponentの3D描画処理を行います
		void draw3D()
		{
			for (auto& c : components_) {
				if (c == nullptr)
				{
					continue;
				}
				c->draw3D();
			}
		}

		//!このEntityについているComponentの2D描画処理を行います
		void draw2D()
		{
			for (auto& c : components_)
			{
				if (c == nullptr)
				{
					continue;
				}
				c->draw2D();
			}
		}

		//!Entityの生存状態を返します
		[[nodiscard]] bool isActive() const { return isActive_; }

		//!Entityを殺します
		void destroy() 
		{
			isActive_ = false; 
		}

		//!Entityが指定したグループに登録されているか返します
		[[nodiscard]] bool hasGroup(Group group) const noexcept
		{
			return groupBitSet_[group];
		}

		//!Entityをグループに登録します
		void addGroup(Group group) noexcept;

		//!Entityをグループから消します
		void removeGroup(Group group) noexcept
		{
			groupBitSet_[group] = false;
		}

		//!Entityに指定したComponentがあるか返します
		template <typename T> [[nodiscard]] bool hasComponent() const
		{
			return componentBitSet_[GetComponentTypeID<T>()];
		}

		/**
		* @brief コンポーネントの追加メソッドです
		* @param args コンポーネントのコンストラクタと同じものになります
		* @return T 追加したコンポーネントのポインタ
		* @details 追加されたらコンポーネントの初期化メソッドが呼ばれます
		* - 重複はできません
		*/
		template <typename T, typename... TArgs> T& addComponent(TArgs&&... args)
		{
			//重複は許可しない
			if (hasComponent<T>())
			{
				std::cerr << "addComponent is failed" << std::endl;
				return getComponent<T>();
			}
			//Tips: std::forward
			//関数テンプレートの引数を転送する。
			T* c(new T(std::forward<TArgs>(args)...));
			c->entity = this;
			std::unique_ptr<ComponentSystem> uPtr(c);
			components_.emplace_back(std::move(uPtr));

			//識別するためのIDと生存フラグをセット
			componentArray_[GetComponentTypeID<T>()] = c;
			componentBitSet_[GetComponentTypeID<T>()] = true;

			c->initialize();
			return *c;
		}

		//!指定したコンポーネントを削除します
		template<typename T> void removeComponent() noexcept
		{
			if (hasComponent<T>())
			{
				getComponent<T>().removeThis();
				componentBitSet_[GetComponentTypeID<T>()] = false;
			}
		}

		/**
		* @brief 登録済みのコンポーネントを取得します
		* @return T 指定したコンポーネントのポインタ
		* @details 失敗した場合例外が発生し、コンソール画面に取得に失敗したコンポーネントが出力されます
		*/
		template<typename T> [[nodiscard]] T& getComponent() const
		{
			if (!hasComponent<T>())
			{
				std::cout << typeid(T).name() << std::endl;
				assert(hasComponent<T>());
			}
			auto ptr(componentArray_[GetComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}
		//!タグを返します
		[[nodiscard]] const std::string& getTag() const
		{
			return tag_;
		}
	};

	/**
	* @brief Entity統括クラスです
	* @details Entityの生成と管理を行います。グループへの登録もこのクラスが行います
	*/
	class EntityManager final
	{
	private:
		std::vector<std::unique_ptr<Entity>> entityes_;
		std::array<std::vector<Entity*>, MaxGroups> groupedEntities_;
	public:
		//!登録されているEntityの初期化を行います
		void initialize()
		{
			for (auto& e : entityes_) e->initialize();
		}
		//!登録されているEntityの更新を行います
		void update()
		{
			for (auto& e : entityes_)
			{
				if (e == nullptr)
				{
					continue;
				}
				e->update();
			}
		}

		//!登録されているEntityの3D描画を行います
		void draw3D()
		{
			for (auto& e : entityes_) e->draw3D();
		}

		/**
		* @brief グループごとの描画を登録順に行います
		* @param MaxGroup 最大グループ数
		*/
		void orderByDraw(const size_t MaxGroup)
		{
			for (auto i(0u); i < MaxGroup; ++i)
			{
				const auto& entity = groupedEntities_[i];
				for (const auto& e : entity)
				{
					e->draw2D();
				}
			}
		}
		//!登録されているEntityの2D描画を行います
		void draw2D()
		{
			for (auto& e : entityes_)
			{
				e->draw2D();
			}
		}
		//!アクティブでないEntityを削除します。必ず更新処理で呼んでください
		void refresh()
		{
			for (auto i(0u); i < MaxGroups; ++i)
			{
				auto& v(groupedEntities_[i]);

				v.erase(std::remove_if(std::begin(v), std::end(v),
					[i](Entity* pEntity)
				{
					return !pEntity->isActive() ||
						!pEntity->hasGroup(i);
				}),
					std::end(v));
			}

			entityes_.erase(std::remove_if(std::begin(entityes_), std::end(entityes_),
				[](const std::unique_ptr<Entity> &pEntity)
			{
				return !pEntity->isActive();
			}),
				std::end(entityes_));
		}

		//!指定したグループに登録されているEntity達を返します
		[[nodiscard]] std::vector<Entity*>& getEntitiesByGroup(Group group)
		{
			return groupedEntities_[group];
		}

		//!Entityを指定したグループに登録します
		void addToGroup(Entity* pEntity, Group group)
		{
			groupedEntities_[group].emplace_back(pEntity);
		}

		/**
		* @brief Entityを生成しそのポインタを返します
		* @param tag 名前の文字列
		* @return Entity& Entityへの参照
		* @details タグを設定しておくとデバッグするときに追いかけやすいため用意してあります
		* 作られたEntityはマネージャーが保持します 
		*/
		[[nodiscard]] Entity& addEntityAddTag(const std::string& tag)
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes_.emplace_back(std::move(uPtr));
			entityes_.back()->tag_ = tag;
			return *e;
		}

		/**
		* @brief Entityを生成しそのポインタを返します。
		* @return Entity& Entityへの参照
		* @details 基本的にこちらを使います。作られたEntityはマネージャーが保持します
		*/
		[[nodiscard]] Entity& addEntity()
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes_.emplace_back(std::move(uPtr));
			entityes_.back()->tag_ = "";
			return *e;
		}
	};

	//!vectorに格納されているエンティティの更新を行います
	void EntitiesUpdate(const std::vector<Entity*>& entities);
	//!vectorに格納されているエンティティの2D描画を行います
	void EntitiesDraw2D(const std::vector<Entity*>& entities);
	//!vectorに格納されているエンティティの3D描画を行います
	void EntitiesDraw3D(const std::vector<Entity*>& entities);

}

