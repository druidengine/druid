#include <raylib.h>

import ui.boardrenderer;

auto main() -> int
{
	// NOLINTBEGIN
	InitWindow(720, 720, "Runestone GUI");
	SetTargetFPS(60);

	runestone_gui::ui::BoardRenderer board_renderer(40);

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		board_renderer.draw();

		EndDrawing();
	}

	CloseWindow();
	// NOLINTEND
	return 0;
}