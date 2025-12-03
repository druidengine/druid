module;

#include <raylib.h>

export module ui.square;

export namespace runestone_gui::ui
{
	class Square
	{
	public:
		Square() = default;

		Square(float px, float py, float size, Color c) : rectangle_{px, py, size, size}, color_(c)
		{
		}

		auto draw() const noexcept -> void
		{
			DrawRectangleRec(rectangle_, color_);

			if (hightighted_ == true)
			{
				DrawRectangleLinesEx(rectangle_, LineThickness, YELLOW);
			}
		}

	private:
		static constexpr auto LineThickness{3.0F};

		Rectangle rectangle_{};
		Color color_{};
		bool hightighted_{false};
	};
}