module;

#include <raylib.h>
#include <array>
#include <ranges>

export module ui.boardrenderer;

import ui.square;

export namespace runestone_gui::ui
{
	class BoardRenderer
	{
	public:
		explicit BoardRenderer(int square_size) : square_size_(square_size)
		{
			init_squares();
		}

		auto draw() const noexcept -> void
		{
			for (const auto &square : squares_)
			{
				square.draw();
			}
		}

	private:
		auto init_squares() noexcept -> void
		{
			for (auto rank : std::ranges::views::iota(0U, Ranks))
			{
				// Repetitively alternate from light square to dark square back to light square.
				bool dark_square = (rank % 2) != 0U;

				for (auto file : std::ranges::views::iota(0U, Files))
				{
					const auto px = file * square_size_;
					const auto py = rank * square_size_;

					const Color color = (dark_square == false) ? LightSquare : DarkSquare;

					squares_.at((rank * Files) + file) =
						Square(static_cast<float>(px), static_cast<float>(py), static_cast<float>(square_size_), color);

					dark_square = !dark_square;
				}
			}
		}

		static constexpr Color LightSquare{240, 217, 181, 255};
		static constexpr Color DarkSquare{181, 136, 99, 255};
		static constexpr std::size_t Files{8};
		static constexpr std::size_t Ranks{8};

		std::array<Square, Files * Ranks> squares_{};
		int square_size_{0};
	};
}
