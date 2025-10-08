#include <chrono>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>

import druid.core.engine;
import druid.graphics.node;
import druid.graphics.node.rectangle;
import druid.graphics.node.text;
import druid.graphics.window;
import druid.graphics.color;

using druid::graphics::Color;

auto main() -> int
try
{
	druid::core::Engine engine;
	auto& window = engine.create_service<druid::graphics::Window>();
	// NOLINTBEGIN

	constexpr auto width{1280};
	constexpr auto height{720};

	auto& root = window.root_node();

	auto& scoreLeft = root.create_node<druid::graphics::NodeText>();
	scoreLeft.set_position({width * 0.3F, height * 0.1F});
	scoreLeft.set_text("0");
	scoreLeft.set_font_size(40);
	scoreLeft.set_color(Color::White);

	auto& scoreRight = root.create_node<druid::graphics::NodeText>();
	scoreRight.set_position({width * 0.7F, height * 0.1F});
	scoreRight.set_text("0");
	scoreRight.set_font_size(40);
	scoreRight.set_color(Color::White);

	auto& paddle1 = root.create_node<druid::graphics::NodeRectangle>();
	paddle1.set_position({width * 0.1, height * 0.5});
	paddle1.set_size({25, 100});
	paddle1.set_color(Color::Green);
	// auto& key = paddle1.create_child<KeyHandler>();

	// auto& action_map = engine.create_child<ActionMap>();
	// auto& move_up = action_map["move_up"];
	// move_up.set_type(KeyBoard::KeyPressed);
	// move_up.set_key(ArrowUp);
	// move_up.set_joystickId(0);
	// move_up.set_axis(Axis::X);
	// move_up.set_range(0, 1);
	// move_up.on_trigger([] {});

	// paddle1.on_update([&move_up] {
	//.  paddle1.set_velocity(0, 0);
	//   if(move_up.is_pressed()) {
	//.    paddle1.set_velocity(0, 1);
	//   }
	// });

	auto& paddle2 = root.create_node<druid::graphics::NodeRectangle>();
	paddle2.set_position({width * 0.9, height * 0.5});
	paddle2.set_size({25, 100});
	paddle2.set_color(Color::Red);

	auto& ball = root.create_node<druid::graphics::NodeRectangle>();
	ball.set_position({width * 0.5, height * 0.5});
	ball.set_size({24, 24});
	ball.set_color(Color::White);

	auto velocity_ball = glm::vec2{50.0F, 0.0F};

	engine.on_update_fixed(
		[&paddle1, &paddle2, &ball, &velocity_ball](std::chrono::steady_clock::duration dt)
		{
			// Move Ball
			using seconds = std::chrono::duration<double>;
			const auto dt_seconds = std::chrono::duration_cast<seconds>(dt).count();
			const auto position = glm::vec2{velocity_ball.x * dt_seconds, velocity_ball.y * dt_seconds};
			ball.set_position(position + ball.get_position());

			const auto ball_top_left = ball.top_left() + ball.get_position();
			const auto ball_bottom_right = ball.bottom_right() + ball.get_position();
			const auto paddle1_top_left = paddle1.top_left() + paddle1.get_position();
			const auto paddle1_bottom_right = paddle1.bottom_right() + paddle1.get_position();
			const auto paddle2_top_left = paddle2.top_left() + paddle2.get_position();
			const auto paddle2_bottom_right = paddle2.bottom_right() + paddle2.get_position();

			// Check Collisions with window borders
			if (ball_top_left.x < 0 || ball_bottom_right.x > width)
			{
				velocity_ball.x = -velocity_ball.x;
			}

			if (ball_top_left.y < 0 || ball_bottom_right.y > height)
			{
				velocity_ball.y = -velocity_ball.y;
			}

			// Check Collisions with paddles
			if (ball_top_left.x < paddle1_bottom_right.x && ball_bottom_right.y > paddle1_top_left.y && ball_top_left.y < paddle1_bottom_right.y)
			{
				velocity_ball.x = -velocity_ball.x;
			}

			if (ball_bottom_right.x > paddle2_top_left.x && ball_bottom_right.y > paddle2_top_left.y && ball_top_left.y < paddle2_bottom_right.y)
			{
				velocity_ball.x = -velocity_ball.x;
			}
		});

	// NOLINTEND

	return engine.run();
}
catch (const std::exception& e)
{
	std::cerr << "Exception: " << e.what() << "\n";
	return EXIT_FAILURE;
}