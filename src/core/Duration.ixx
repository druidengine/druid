module;

#include <chrono>

export module druid.core.duration;

export namespace druid::core
{
	/// @brief Type alias for fractional seconds using double precision.
	/// This is useful for physics and math calculations that require fractional time values.
	using seconds = std::chrono::duration<double>;

	/// @brief Convert a steady_clock::duration to fractional seconds.
	/// @param x The duration to convert.
	/// @return The duration in seconds as a double-precision floating-point value.
	[[nodiscard]] constexpr auto to_seconds(std::chrono::steady_clock::duration x) noexcept -> seconds
	{
		return std::chrono::duration_cast<seconds>(x);
	}

	/// @brief Convert a steady_clock::duration to milliseconds.
	/// @param x The duration to convert.
	/// @return The duration in milliseconds.
	[[nodiscard]] constexpr auto to_milliseconds(std::chrono::steady_clock::duration x) noexcept -> std::chrono::milliseconds
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(x);
	}

	/// @brief Convert a steady_clock::duration to microseconds.
	/// @param x The duration to convert.
	/// @return The duration in microseconds.
	[[nodiscard]] constexpr auto to_microseconds(std::chrono::steady_clock::duration x) noexcept -> std::chrono::microseconds
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(x);
	}

	/// @brief Convert a steady_clock::duration to nanoseconds.
	/// @param x The duration to convert.
	/// @return The duration in nanoseconds.
	[[nodiscard]] constexpr auto to_nanoseconds(std::chrono::steady_clock::duration x) noexcept -> std::chrono::nanoseconds
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(x);
	}
}
