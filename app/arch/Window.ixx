module;

#include <flecs.h>
#include <raylib.h>
#include <string_view>

export module window;
export import rectangle;

namespace druid::arch
{
	export class Window
	{
	public:
		Window(int width, int height, std::string_view x)
		{
			InitWindow(width, height, x.data());
		}

		~Window()
		{
			CloseWindow();
		}

		auto open() const -> bool
		{
			return !WindowShouldClose();
		}

		auto clear(Color x = BLACK) const -> void
		{
			BeginDrawing();
			ClearBackground(x);
		}

		auto display() -> void
		{
			DrawFPS(0, 0);
			EndDrawing();
		}

		auto draw(const flecs::world& x) const
		{
			x.each([](Rectangle& x) { DrawRectangle(x.x, x.y, x.width, x.height, x.color); });
		}
	};
}