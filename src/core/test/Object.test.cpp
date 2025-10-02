#include <gtest/gtest.h>

import druid.core.object;

using druid::core::Object;

TEST(Object, name)
{
	Object object;
	object.set_name("test");
	EXPECT_EQ(object.get_name(), "test");
}

TEST(Object, addChild)
{
	Object object;
	EXPECT_NO_THROW(object.add_child(std::make_unique<Object>()));
}

TEST(Object, create_child)
{
	Object object;

	auto& child = object.create_child("test");
	EXPECT_EQ(child.get_name(), "test");
}

TEST(Object, remove)
{
	Object object;
	EXPECT_EQ(object.remove(), nullptr);

	(void)object.create_child("one");
	auto& two = object.create_child("two");
	(void)object.create_child("three");

	EXPECT_EQ(two.get_name(), "two");

	EXPECT_EQ(object.children().size(), 3U);

	auto child = two.remove();
	EXPECT_EQ(child->get_name(), "two");

	EXPECT_EQ(object.children().size(), 2U);
}

TEST(Object, find_child)
{
	Object object;
	auto& one = object.create_child("one");
	auto& two = object.create_child("two");
	auto& three = object.create_child("three");

	auto* child = object.find_child("one");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &one);

	child = object.find_child("two");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &two);

	child = object.find_child("three");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &three);
}