module;

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

export module druid.core.entity;

export namespace druid::core
{
	class Entity
	{
	public:
		explicit Entity(entt::registry& x) : registry_{&x}, id_{registry_->create()}
		{
		}

		template <typename T, typename... Args>
		[[nodiscard]] auto add_component(Args&&... args) -> T&
		{
			return registry_->emplace<T>(id_, std::forward<Args>(args)...);
		}

		template <typename T>
		auto remove_component() -> void
		{
			registry_->erase<T>(id_);
		}

		template <typename... Args>
		[[nodiscard]] auto has_components() const -> bool
		{
			return registry_->all_of<Args...>(id_);
		}

		[[nodiscard]] auto valid() const -> bool
		{
			return id_ != entt::null;
		}

	private:
		entt::registry* registry_{};
		entt::entity id_{entt::null};
	};
}