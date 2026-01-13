module;

#include <SDL3/SDL.h>
#include <string>

export module druid.sdl.Renderer;

import druid.graphics.Color;
import druid.graphics.Renderer;

export namespace druid::sdl
{
	/// @class Renderer
	/// @brief SDL3-backed implementation of the Renderer interface.
	///
	/// This class adapts the abstract `druid::graphics::Renderer` API to the
	/// SDL3 graphics library. It translates engine-agnostic draw commands
	/// into SDL3 function calls.
	///
	/// `Renderer` is intended to be owned and managed by higher-level services
	/// (such as `graphics::Window`) and does not manage window creation or
	/// lifetime itself.
	class Renderer : public druid::graphics::Renderer
	{
	public:
		/// @brief Construct renderer with SDL renderer handle.
		/// @param renderer SDL renderer handle owned by the window.
		explicit Renderer(SDL_Renderer* renderer) : renderer_{renderer}
		{
		}

		/// @brief Virtual destructor.
		~Renderer() override = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		auto operator=(const Renderer&) -> Renderer& = delete;
		auto operator=(Renderer&&) noexcept -> Renderer& = delete;

		/// @brief Begin a new rendering frame.
		///
		/// Clears the background to the specified color and begins an SDL3
		/// drawing context.
		///
		/// @param clear Background clear color.
		auto begin(druid::graphics::Color clear) -> void override
		{
			SDL_SetRenderDrawColor(renderer_, clear.red(), clear.green(), clear.blue(), clear.alpha());
			SDL_RenderClear(renderer_);
		}

		/// @brief End the current rendering frame.
		///
		/// Finalizes the SDL3 drawing context and presents the frame.
		auto end() -> void override
		{
			SDL_RenderPresent(renderer_);
		}

		/// @brief Draw a filled rectangle.
		///
		/// @param x X-coordinate of the rectangle origin.
		/// @param y Y-coordinate of the rectangle origin.
		/// @param width Rectangle width.
		/// @param height Rectangle height.
		/// @param color Fill color.
		auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color color) -> void override
		{
			SDL_SetRenderDrawColor(renderer_, color.red(), color.green(), color.blue(), color.alpha());

			const SDL_FRect rect{.x = x, .y = y, .w = width, .h = height};

			SDL_RenderFillRect(renderer_, &rect);
		}

		/// @brief Draw a text string.
		///
		/// @param x X-coordinate of the text origin.
		/// @param y Y-coordinate of the text origin.
		/// @param text UTF-8 text string to render.
		/// @param font_size Font size in pixels.
		/// @param color Text color.
		auto draw_text(float /*x*/, float /*y*/, const std::string& /*text*/, int /*font_size*/, druid::graphics::Color /*color*/) -> void override
		{
			// SDL3 does not have built-in text rendering; would require SDL_ttf
			// For now, this is a stub implementation.
		}

	private:
		SDL_Renderer* renderer_;
	};
}
