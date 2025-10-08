module;

#include <raylib.h>
#include <string>

export module druid.graphics.renderer.raylib;
import druid.graphics.renderer;

namespace druid::graphics::renderer
{
	export class Raylib : public Renderer
	{
	public:
		Raylib() = default;
		~Raylib() override = default;
		Raylib(const Raylib&) = delete;
		Raylib(Raylib&&) noexcept = delete;
		auto operator=(const Raylib&) -> Raylib& = delete;
		auto operator=(Raylib&&) noexcept -> Raylib& = delete;

		auto begin(druid::graphics::Color clear) -> void override
		{
			const ::Color rl_color{.r = clear.red(), .g = clear.green(), .b = clear.blue(), .a = clear.alpha()};
			ClearBackground(rl_color);
			BeginDrawing();
		}

		auto end() -> void override
		{
			DrawFPS(0, 0);
			EndDrawing();
		}

		auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color color) -> void override
		{
			const ::Color rl_color{.r = color.red(), .g = color.green(), .b = color.blue(), .a = color.alpha()};
			const Rectangle rect{.x = x, .y = y, .width = width, .height = height};
			DrawRectangleRec(rect, rl_color);
		}

		auto draw_text(float x, float y, const std::string& text, int font_size, druid::graphics::Color color) -> void override
		{
			const ::Color rl_color{.r = color.red(), .g = color.green(), .b = color.blue(), .a = color.alpha()};
			DrawText(text.c_str(), static_cast<int>(x), static_cast<int>(y), font_size, rl_color);
		}
	};
}