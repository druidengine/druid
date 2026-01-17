module;

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <ranges>
#include <vector>

export module druid.widgets.Widget;

import druid.core.Engine;
import druid.core.Object;

export namespace druid::widgets
{
	/// @class Widget
	/// @brief Base class for UI widgets with bounding box and hit testing.
	///
	/// `Widget` extends `druid::core::Object` to provide UI-specific functionality
	/// including spatial bounds (bounding box) and point containment testing.
	/// Widgets can be organized hierarchically, and hit testing will traverse
	/// the hierarchy to find the deepest child that contains a given point.
	class Widget : public core::Object
	{
	public:
		/// @brief Construct a widget associated with the given engine.
		/// @param engine Owning engine instance.
		explicit Widget(core::Engine& engine) : core::Object(engine)
		{
			on_child_removed(
				[this](core::Object* child)
				{
					if (auto* widget = dynamic_cast<Widget*>(child))
					{
						std::erase(children_widget_, widget);
					}
				});
		}

		/// @brief Virtual destructor.
		~Widget() override = default;

		Widget(const Widget&) = delete;
		auto operator=(const Widget&) -> Widget& = delete;
		Widget(Widget&&) noexcept = delete;
		auto operator=(Widget&&) noexcept -> Widget& = delete;

		/// @brief Set the position of the widget's bounding box.
		/// @param position Top-left corner position (x, y).
		auto set_position(glm::vec2 position) -> void
		{
			position_ = position;
		}

		/// @brief Get the position of the widget's bounding box.
		/// @return Top-left corner position (x, y).
		[[nodiscard]] auto get_position() const noexcept -> glm::vec2
		{
			return position_;
		}

		/// @brief Set the size of the widget's bounding box.
		/// @param size Size of the widget (width, height).
		auto set_size(glm::vec2 size) -> void
		{
			size_ = size;
		}

		/// @brief Get the size of the widget's bounding box.
		/// @return Size of the widget (width, height).
		[[nodiscard]] auto get_size() const noexcept -> glm::vec2
		{
			return size_;
		}

		/// @brief Check if a point is contained within this widget's bounding box.
		///
		/// Uses axis-aligned bounding box (AABB) containment test.
		/// A point is considered inside if it falls within the rectangle defined by
		/// the widget's position and size.
		///
		/// @param point The point to test (x, y).
		/// @return True if the point is within the widget's bounds, false otherwise.
		[[nodiscard]] auto contains(glm::vec2 point) const noexcept -> bool
		{
			return point.x >= position_.x && point.x <= position_.x + size_.x && point.y >= position_.y && point.y <= position_.y + size_.y;
		}

		/// @brief Add a widget as a child of this widget.
		///
		/// This method stores the widget pointer for efficient widget-specific operations
		/// (like hit testing) and transfers ownership to the base Object hierarchy.
		///
		/// @param widget Unique pointer to the widget to add as a child.
		auto add_widget(std::unique_ptr<Widget> widget) -> void
		{
			if (widget == nullptr)
			{
				return;
			}

			children_widget_.emplace_back(widget.get());

			// Transfer ownership to the base Object class
			// The widget will be automatically added to children_widget_ via the on_child_added signal
			add_child(std::move(widget));
		}

		/// @brief Get the list of widget children.
		/// @return Read-only reference to the vector of widget children.
		[[nodiscard]] auto children_widget() const noexcept -> const std::vector<Widget*>&
		{
			return children_widget_;
		}

		/// @brief Find the widget child that contains the given point (const version).
		///
		/// Performs a depth-first search through the widget hierarchy to find the
		/// deepest (most specific) widget that contains the point. If multiple children
		/// contain the point, the last one in the children list takes precedence.
		///
		/// @param point The point to test (x, y).
		/// @return Pointer to the widget that contains the point, or nullptr if no widget
		///         contains the point. Returns `this` if no child contains the point but
		///         this widget does.
		[[nodiscard]] auto widget_at(glm::vec2 point) const -> const Widget*
		{
			// Point is not within this widget or any of its children
			if (!contains(point))
			{
				return nullptr;
			}

			// Check widget children first (depth-first search)
			// Iterate in reverse to prioritize later children (drawn on top)
			for (const auto* widget : std::views::reverse(children_widget_))
			{
				// Recursively check if the child or any of its descendants contains the point
				if (const auto* found = widget->widget_at(point))
				{
					return found;
				}
			}

			// If no child contains the point this widget does.
			return this;
		}

	private:
		glm::vec2 position_{0.0F, 0.0F};
		glm::vec2 size_{0.0F, 0.0F};
		std::vector<Widget*> children_widget_;
	};
}
