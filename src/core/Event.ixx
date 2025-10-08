module;

#include <cstdint>
#include <variant>

export module druid.core.event;

export namespace druid::core
{
	struct EventKeyboard
	{
		enum class Type : std::uint8_t
		{
			None,
			KeyPressed,
			KeyReleased
		};

		enum class Key : std::uint8_t
		{
			None,
			ArrowUp,
			ArrowDown,
			ArrowLeft,
			ArrowRight
		};

		Type type{Type::None};
		Key key{Key::None};
	};

	struct EventWindow
	{
		enum class Type : std::uint8_t
		{
			None,
			Moved,
			Resized,
			Closed
		};

		Type type{Type::None};
	};

	struct EventMouse
	{
		enum class Type : std::uint8_t
		{
			None,
			ButtonPressed,
			ButtonReleased,
			Moved
		};

		enum class Button : std::uint8_t
		{
			None,
			Left,
			Right,
			Middle
		};

		Type type{Type::None};
		Button button{Button::None};
	};

	using Event = std::variant<std::monostate, EventKeyboard, EventWindow, EventMouse>;
}