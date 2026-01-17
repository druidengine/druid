#include <gtest/gtest.h>
#include <glm/glm.hpp>

import druid.core.Object;
import druid.core.Engine;
import druid.widgets.Widget;

using druid::core::Engine;
using druid::widgets::Widget;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST(Widget, construction)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);
	EXPECT_NE(widget, nullptr);

	// Default position and size should be zero
	EXPECT_EQ(widget->get_position().x, 0.0F);
	EXPECT_EQ(widget->get_position().y, 0.0F);
	EXPECT_EQ(widget->get_size().x, 0.0F);
	EXPECT_EQ(widget->get_size().y, 0.0F);
}

TEST(Widget, position)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	const auto pos = glm::vec2{100.0F, 200.0F};
	widget->set_position(pos);

	EXPECT_EQ(widget->get_position().x, 100.0F);
	EXPECT_EQ(widget->get_position().y, 200.0F);
}

TEST(Widget, size)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	const auto size = glm::vec2{150.0F, 250.0F};
	widget->set_size(size);

	EXPECT_EQ(widget->get_size().x, 150.0F);
	EXPECT_EQ(widget->get_size().y, 250.0F);
}

TEST(Widget, contains_point_inside)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	widget->set_position({100.0F, 100.0F});
	widget->set_size({200.0F, 150.0F});

	// Point inside the widget
	const auto point = glm::vec2{150.0F, 150.0F};
	EXPECT_TRUE(widget->contains(point));

	// Point at the edges (should be inside)
	EXPECT_TRUE(widget->contains({100.0F, 100.0F})); // Top-left corner
	EXPECT_TRUE(widget->contains({300.0F, 250.0F})); // Bottom-right corner
	EXPECT_TRUE(widget->contains({200.0F, 175.0F})); // Center
}

TEST(Widget, contains_point_outside)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	widget->set_position({100.0F, 100.0F});
	widget->set_size({200.0F, 150.0F});

	// Points outside the widget
	EXPECT_FALSE(widget->contains({50.0F, 150.0F}));  // Left
	EXPECT_FALSE(widget->contains({350.0F, 150.0F})); // Right
	EXPECT_FALSE(widget->contains({200.0F, 50.0F}));  // Above
	EXPECT_FALSE(widget->contains({200.0F, 300.0F})); // Below
	EXPECT_FALSE(widget->contains({99.0F, 99.0F}));	  // Just outside top-left
	EXPECT_FALSE(widget->contains({301.0F, 251.0F})); // Just outside bottom-right
}

TEST(Widget, add_widget)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);

	auto child = std::make_unique<Widget>(engine);
	child->set_name("child");

	parent->add_widget(std::move(child));

	// Check that child was added to Object children
	EXPECT_EQ(std::size(parent->children()), 1U);

	// Check that child was added to widget children
	EXPECT_EQ(std::size(parent->children_widget()), 1U);

	// Verify it's the correct child
	auto* found = parent->find_child("child");
	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found->get_name(), "child");
}

TEST(Widget, add_widget_nullptr)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);

	// Adding nullptr should be safe (no-op)
	parent->add_widget(nullptr);

	EXPECT_EQ(std::size(parent->children()), 0U);
	EXPECT_EQ(std::size(parent->children_widget()), 0U);
}

TEST(Widget, children_widget)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);

	// Add multiple widget children
	for (int i = 0; i < 3; ++i)
	{
		auto child = std::make_unique<Widget>(engine);
		parent->add_widget(std::move(child));
	}

	const auto& widget_children = parent->children_widget();
	EXPECT_EQ(std::size(widget_children), 3U);
}

TEST(Widget, get_widget_at_self)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	widget->set_position({100.0F, 100.0F});
	widget->set_size({200.0F, 150.0F});

	// Point inside widget with no children should return self
	const auto point = glm::vec2{150.0F, 150.0F};
	const auto* found = widget->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, widget.get());
}

TEST(Widget, get_widget_at_outside)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	widget->set_position({100.0F, 100.0F});
	widget->set_size({200.0F, 150.0F});

	// Point outside widget should return nullptr
	const auto point = glm::vec2{50.0F, 50.0F};
	const auto* found = widget->widget_at(point);

	EXPECT_EQ(found, nullptr);
}

TEST(Widget, get_widget_at_child)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);
	parent->set_position({0.0F, 0.0F});
	parent->set_size({400.0F, 400.0F});

	auto child = std::make_unique<Widget>(engine);
	child->set_name("child");
	child->set_position({100.0F, 100.0F});
	child->set_size({100.0F, 100.0F});

	auto* child_ptr = child.get();
	parent->add_widget(std::move(child));

	// Point inside child should return child
	const auto point = glm::vec2{150.0F, 150.0F};
	const auto* found = parent->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, child_ptr);
	EXPECT_EQ(found->get_name(), "child");
}

TEST(Widget, get_widget_at_parent)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);
	parent->set_position({0.0F, 0.0F});
	parent->set_size({400.0F, 400.0F});

	auto child = std::make_unique<Widget>(engine);
	child->set_position({100.0F, 100.0F});
	child->set_size({100.0F, 100.0F});

	parent->add_widget(std::move(child));

	// Point inside parent but outside child should return parent
	const auto point = glm::vec2{50.0F, 50.0F};
	const auto* found = parent->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, parent.get());
}

TEST(Widget, get_widget_at_nested_children)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);
	parent->set_position({0.0F, 0.0F});
	parent->set_size({400.0F, 400.0F});

	auto child = std::make_unique<Widget>(engine);
	child->set_name("child");
	child->set_position({100.0F, 100.0F});
	child->set_size({200.0F, 200.0F});

	auto grandchild = std::make_unique<Widget>(engine);
	grandchild->set_name("grandchild");
	grandchild->set_position({150.0F, 150.0F});
	grandchild->set_size({50.0F, 50.0F});

	auto* grandchild_ptr = grandchild.get();
	child->add_widget(std::move(grandchild));
	parent->add_widget(std::move(child));

	// Point inside grandchild should return grandchild
	const auto point = glm::vec2{175.0F, 175.0F};
	const auto* found = parent->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, grandchild_ptr);
	EXPECT_EQ(found->get_name(), "grandchild");
}

TEST(Widget, get_widget_at_overlapping_children)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);
	parent->set_position({0.0F, 0.0F});
	parent->set_size({400.0F, 400.0F});

	// First child
	auto child1 = std::make_unique<Widget>(engine);
	child1->set_name("child1");
	child1->set_position({100.0F, 100.0F});
	child1->set_size({150.0F, 150.0F});

	// Second child overlapping the first
	auto child2 = std::make_unique<Widget>(engine);
	child2->set_name("child2");
	child2->set_position({150.0F, 150.0F});
	child2->set_size({150.0F, 150.0F});

	auto* child2_ptr = child2.get();
	parent->add_widget(std::move(child1));
	parent->add_widget(std::move(child2));

	// Point in overlapping region should return the last added child (child2)
	const auto point = glm::vec2{175.0F, 175.0F};
	const auto* found = parent->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, child2_ptr);
	EXPECT_EQ(found->get_name(), "child2");
}

TEST(Widget, get_widget_at_const)
{
	Engine engine;
	auto widget = std::make_unique<Widget>(engine);

	widget->set_position({100.0F, 100.0F});
	widget->set_size({200.0F, 150.0F});

	const Widget* const_widget = widget.get();

	// Point inside widget
	const auto point = glm::vec2{150.0F, 150.0F};
	const auto* found = const_widget->widget_at(point);

	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found, const_widget);
}

TEST(Widget, child_removal_updates_widget_children)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);

	auto child = std::make_unique<Widget>(engine);
	child->set_name("child");

	parent->add_widget(std::move(child));

	EXPECT_EQ(std::size(parent->children_widget()), 1U);

	// Remove the child
	auto* child_obj = parent->find_child("child");
	ASSERT_NE(child_obj, nullptr);
	auto removed = child_obj->remove();

	// Widget children should be updated
	EXPECT_EQ(std::size(parent->children_widget()), 0U);
	EXPECT_EQ(std::size(parent->children()), 0U);
}

TEST(Widget, mixed_children_only_widgets_tracked)
{
	Engine engine;
	auto parent = std::make_unique<Widget>(engine);

	// Add a widget child
	auto widget_child = std::make_unique<Widget>(engine);
	widget_child->set_name("widget_child");
	parent->add_widget(std::move(widget_child));

	// Add a non-widget Object child directly
	auto object_child = std::make_unique<druid::core::Object>(engine);
	object_child->set_name("object_child");
	parent->add_child(std::move(object_child));

	// Should have 2 Object children
	EXPECT_EQ(std::size(parent->children()), 2U);

	// Should have only 1 Widget child
	EXPECT_EQ(std::size(parent->children_widget()), 1U);

	// Remove all children (avoid modifying collection during iteration)
	while (!std::empty(parent->children()))
	{
		auto c = parent->children().front()->remove();
		EXPECT_NE(c, nullptr);
	}

	EXPECT_TRUE(std::empty(parent->children()));
	EXPECT_TRUE(std::empty(parent->children_widget()));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
