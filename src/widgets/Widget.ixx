module;

#include <glm/glm.hpp>

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
			return point.x >= position_.x && point.x <= position_.x + size_.x && point.y >= position_.y &&
			       point.y <= position_.y + size_.y;
		}

		/// @brief Find the widget child that contains the given point.
		///
		/// Performs a depth-first search through the widget hierarchy to find the
		/// deepest (most specific) widget that contains the point. If multiple children
		/// contain the point, the last one in the children list takes precedence.
		///
		/// @param point The point to test (x, y).
		/// @return Pointer to the widget that contains the point, or nullptr if no widget
		///         contains the point. Returns `this` if no child contains the point but
		///         this widget does.
		[[nodiscard]] auto get_widget_at(glm::vec2 point) -> Widget*
		{
			// Check children first (depth-first search)
			// Iterate in reverse to prioritize later children (drawn on top)
			const auto& child_list = children();
			for (auto i = child_list.size(); i > 0; --i)
			{
				// Try to cast child to Widget
				if (auto* child_widget = dynamic_cast<Widget*>(child_list[i - 1].get()))
				{
					// Recursively check if the child or any of its descendants contains the point
					if (auto* found = child_widget->get_widget_at(point))
					{
						return found;
					}
				}
			}

			// If no child contains the point, check if this widget does
			if (contains(point))
			{
				return this;
			}

			// Point is not within this widget or any of its children
			return nullptr;
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
		[[nodiscard]] auto get_widget_at(glm::vec2 point) const -> const Widget*
		{
			// Check children first (depth-first search)
			// Iterate in reverse to prioritize later children (drawn on top)
			const auto& child_list = children();
			for (auto i = child_list.size(); i > 0; --i)
			{
				// Try to cast child to Widget
				if (const auto* child_widget = dynamic_cast<const Widget*>(child_list[i - 1].get()))
				{
					// Recursively check if the child or any of its descendants contains the point
					if (const auto* found = child_widget->get_widget_at(point))
					{
						return found;
					}
				}
			}

			// If no child contains the point, check if this widget does
			if (contains(point))
			{
				return this;
			}

			// Point is not within this widget or any of its children
			return nullptr;
		}

	private:
		glm::vec2 position_{0.0F, 0.0F}; ///< Top-left corner of the bounding box.
		glm::vec2 size_{0.0F, 0.0F};     ///< Size of the bounding box (width, height).
	};
}
