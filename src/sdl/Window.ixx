module;

#include <SDL3/SDL.h>
#include <chrono>

export module druid.sdl.Window;

import druid.core.Engine;
import druid.core.Event;
import druid.graphics.Color;
import druid.graphics.Node;
import druid.graphics.Renderer;
import druid.graphics.Window;

namespace
{
	[[nodiscard]] auto ConvertSDLKey(SDL_Keycode x) -> druid::core::EventKeyboard::Key
	{
		using Key = druid::core::EventKeyboard::Key;

		switch (x)
		{
			case SDLK_0:
				return Key::Zero;
			case SDLK_1:
				return Key::One;
			case SDLK_2:
				return Key::Two;
			case SDLK_3:
				return Key::Three;
			case SDLK_4:
				return Key::Four;
			case SDLK_5:
				return Key::Five;
			case SDLK_6:
				return Key::Six;
			case SDLK_7:
				return Key::Seven;
			case SDLK_8:
				return Key::Eight;
			case SDLK_9:
				return Key::Nine;
			case SDLK_A:
				return Key::A;
			case SDLK_B:
				return Key::B;
			case SDLK_C:
				return Key::C;
			case SDLK_D:
				return Key::D;
			case SDLK_E:
				return Key::E;
			case SDLK_F:
				return Key::F;
			case SDLK_G:
				return Key::G;
			case SDLK_H:
				return Key::H;
			case SDLK_I:
				return Key::I;
			case SDLK_J:
				return Key::J;
			case SDLK_K:
				return Key::K;
			case SDLK_L:
				return Key::L;
			case SDLK_M:
				return Key::M;
			case SDLK_N:
				return Key::N;
			case SDLK_O:
				return Key::O;
			case SDLK_P:
				return Key::P;
			case SDLK_Q:
				return Key::Q;
			case SDLK_R:
				return Key::R;
			case SDLK_S:
				return Key::S;
			case SDLK_T:
				return Key::T;
			case SDLK_U:
				return Key::U;
			case SDLK_V:
				return Key::V;
			case SDLK_W:
				return Key::W;
			case SDLK_X:
				return Key::X;
			case SDLK_Y:
				return Key::Y;
			case SDLK_Z:
				return Key::Z;
			case SDLK_SPACE:
				return Key::Space;
			case SDLK_APOSTROPHE:
				return Key::Apostrophe;
			case SDLK_COMMA:
				return Key::Comma;
			case SDLK_MINUS:
				return Key::Minus;
			case SDLK_PERIOD:
				return Key::Period;
			case SDLK_SLASH:
				return Key::Slash;
			case SDLK_SEMICOLON:
				return Key::Semicolon;
			case SDLK_EQUALS:
				return Key::Equal;
			case SDLK_LEFTBRACKET:
				return Key::LeftBracket;
			case SDLK_BACKSLASH:
				return Key::Backslash;
			case SDLK_RIGHTBRACKET:
				return Key::RightBracket;
			case SDLK_GRAVE:
				return Key::Grave;
			case SDLK_ESCAPE:
				return Key::Escape;
			case SDLK_RETURN:
				return Key::Enter;
			case SDLK_TAB:
				return Key::Tab;
			case SDLK_BACKSPACE:
				return Key::Backspace;
			case SDLK_INSERT:
				return Key::Insert;
			case SDLK_DELETE:
				return Key::Delete;
			case SDLK_RIGHT:
				return Key::Right;
			case SDLK_LEFT:
				return Key::Left;
			case SDLK_DOWN:
				return Key::Down;
			case SDLK_UP:
				return Key::Up;
			case SDLK_PAGEUP:
				return Key::PageUp;
			case SDLK_PAGEDOWN:
				return Key::PageDown;
			case SDLK_HOME:
				return Key::Home;
			case SDLK_END:
				return Key::End;
			case SDLK_CAPSLOCK:
				return Key::CapsLock;
			case SDLK_SCROLLLOCK:
				return Key::ScrollLock;
			case SDLK_NUMLOCKCLEAR:
				return Key::NumLock;
			case SDLK_PRINTSCREEN:
				return Key::PrintScreen;
			case SDLK_PAUSE:
				return Key::Pause;
			case SDLK_F1:
				return Key::F1;
			case SDLK_F2:
				return Key::F2;
			case SDLK_F3:
				return Key::F3;
			case SDLK_F4:
				return Key::F4;
			case SDLK_F5:
				return Key::F5;
			case SDLK_F6:
				return Key::F6;
			case SDLK_F7:
				return Key::F7;
			case SDLK_F8:
				return Key::F8;
			case SDLK_F9:
				return Key::F9;
			case SDLK_F10:
				return Key::F10;
			case SDLK_F11:
				return Key::F11;
			case SDLK_F12:
				return Key::F12;
			case SDLK_LSHIFT:
				return Key::LeftShift;
			case SDLK_LCTRL:
				return Key::LeftControl;
			case SDLK_LALT:
				return Key::LeftAlt;
			case SDLK_LGUI:
				return Key::LeftSuper;
			case SDLK_RSHIFT:
				return Key::RightShift;
			case SDLK_RCTRL:
				return Key::RightControl;
			case SDLK_RALT:
				return Key::RightAlt;
			case SDLK_RGUI:
				return Key::RightSuper;
			case SDLK_MENU:
				return Key::Menu;
			case SDLK_KP_0:
				return Key::Kp0;
			case SDLK_KP_1:
				return Key::Kp1;
			case SDLK_KP_2:
				return Key::Kp2;
			case SDLK_KP_3:
				return Key::Kp3;
			case SDLK_KP_4:
				return Key::Kp4;
			case SDLK_KP_5:
				return Key::Kp5;
			case SDLK_KP_6:
				return Key::Kp6;
			case SDLK_KP_7:
				return Key::Kp7;
			case SDLK_KP_8:
				return Key::Kp8;
			case SDLK_KP_9:
				return Key::Kp9;
			case SDLK_KP_DECIMAL:
				return Key::KpDecimal;
			case SDLK_KP_DIVIDE:
				return Key::KpDivide;
			case SDLK_KP_MULTIPLY:
				return Key::KpMultiply;
			case SDLK_KP_MINUS:
				return Key::KpSubtract;
			case SDLK_KP_PLUS:
				return Key::KpAdd;
			case SDLK_KP_ENTER:
				return Key::KpEnter;
			case SDLK_KP_EQUALS:
				return Key::KpEqual;
			case SDLK_VOLUMEUP:
				return Key::VolumeUp;
			case SDLK_VOLUMEDOWN:
				return Key::VolumeDown;
			default:
				return Key::None;
		}
	}
}

export namespace druid::sdl
{
	/// @class Window
	/// @brief SDL3-based window implementation.
	///
	/// `Window` provides a concrete implementation of the `druid::graphics::Window` interface
	/// using the SDL3 library for window creation, input handling, and rendering.
	class Window : public druid::graphics::Window
	{
	public:
		/// @brief Construct the SDL3 window service and bind it to the engine.
		///
		/// Initializes the SDL3 window with default dimensions.
		/// Renderer must be set separately via set_renderer().
		///
		/// @param x Owning engine instance.
		explicit Window(druid::core::Engine& x) : druid::graphics::Window{x}
		{
			if (!SDL_Init(SDL_INIT_VIDEO))
			{
				// Handle error
				return;
			}

			window_ = SDL_CreateWindow(get_title().data(), width_, height_, SDL_WINDOW_RESIZABLE);

			if (!window_)
			{
				SDL_Quit();
				return;
			}

			last_title_ = get_title();
		}

		/// @brief Destroy the SDL3 window service.
		///
		/// Closes the SDL3 window and releases all resources.
		~Window() override
		{
			if (window_)
			{
				SDL_DestroyWindow(window_);
			}

			SDL_Quit();
		}

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		/// @brief Per-frame update hook.
		///
		/// Polls SDL3 for window events and input, and dispatches them to the engine
		/// event system.
		auto update(std::chrono::steady_clock::duration /*x*/) -> void override
		{
			// Check if title has changed and update SDL3 window
			if (get_title() != last_title_)
			{
				last_title_ = get_title();
				SDL_SetWindowTitle(window_, last_title_.c_str());
			}

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_EVENT_QUIT:
					{
						using druid::core::EventWindow;
						EventWindow e{.type = EventWindow::Type::Closed};
						engine().event(e);
						break;
					}

					case SDL_EVENT_KEY_DOWN:
					{
						using druid::core::EventKeyboard;
						EventKeyboard e{.type = EventKeyboard::Type::KeyPressed, .key = ConvertSDLKey(event.key.key)};
						engine().event(e);
						break;
					}

					case SDL_EVENT_KEY_UP:
					{
						using druid::core::EventKeyboard;
						EventKeyboard e{.type = EventKeyboard::Type::KeyReleased, .key = ConvertSDLKey(event.key.key)};
						engine().event(e);
						break;
					}

					default:
						break;
				}
			}
		}

		/// @brief End-of-frame hook.
		///
		/// Renders the scene graph using the SDL3 renderer.
		auto update_end() -> void override
		{
			// Render the scene
			if (auto* renderer = get_renderer())
			{
				renderer->begin(druid::graphics::Color::Black);
				root_node().draw(*renderer);
				renderer->end();
			}
		}

	private:
		SDL_Window* window_{nullptr};
		std::string last_title_{DefaultTitle};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
	};
}
