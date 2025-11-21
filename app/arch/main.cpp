#include <flecs.h>
#include <raylib.h>
#include <glm/glm.hpp>

import window;

using DruidRectangle = druid::arch::Rectangle;
using druid::arch::Window;

auto main() -> int
{
	constexpr auto width{1280};
	constexpr auto height{720};

	flecs::world entities;

	auto enemy = entities.prefab("enemy");
	enemy.set<DruidRectangle>({.x = 0, .y = 0, .width = 25, .height = 25, .color = RED});

	for (auto i = 0; i < 1000; i++)
	{
		auto e = entities.entity().is_a(enemy);
		auto& r = e.get_mut<DruidRectangle>();

		float angle = GetRandomValue(0, 360) * DEG2RAD;
		float radius = GetRandomValue(0, height / 2);
		r.x = width / 2 + radius * cosf(angle) - r.width / 2;
		r.y = height / 2 + radius * sinf(angle) - r.height / 2;
	}

	flecs::world controls;

	Window window{width, height, "druid-arch"};

	while (window.open())
	{
		// SceneGraph graph;
		// graph.dirty()
		// graph.update_transforms();

		// WindowEvents events;
		// events.handle(entities);

		window.clear();
		window.draw(entities);
		window.draw(controls);
		window.display();
	}

	return 0;
}
