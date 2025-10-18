#include <gtest/gtest.h>

import druid.core.engine;
import druid.core.entity;

TEST(Entity, create)
{
	druid::core::Engine engine;

	auto entity = engine.create_entity();
	EXPECT_TRUE(entity.valid());
}

TEST(Entity, component)
{
	struct Transform
	{
		int x{};
		int y{};
	};

	druid::core::Engine engine;

	auto entity = engine.create_entity();
	EXPECT_FALSE(entity.has_components<Transform>());
	const auto& transform = entity.add_component<Transform>();
	EXPECT_EQ(transform.x, 0);
	EXPECT_EQ(transform.y, 0);
	EXPECT_TRUE(entity.has_components<Transform>());
	entity.remove_component<Transform>();
	EXPECT_FALSE(entity.has_components<Transform>());
}
