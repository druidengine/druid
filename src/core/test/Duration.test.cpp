#include <gtest/gtest.h>
#include <chrono>

import druid.core.duration;

using namespace std::chrono_literals;
using druid::core::seconds;
using druid::core::to_seconds;
using druid::core::to_milliseconds;
using druid::core::to_microseconds;
using druid::core::to_nanoseconds;

TEST(Duration, seconds_type_alias)
{
	// Test that seconds type can hold fractional values
	seconds s{1.5};
	EXPECT_DOUBLE_EQ(s.count(), 1.5);

	seconds s2{0.001};
	EXPECT_DOUBLE_EQ(s2.count(), 0.001);
}

TEST(Duration, to_seconds_from_milliseconds)
{
	auto duration = 1500ms;
	auto result = to_seconds(duration);
	EXPECT_DOUBLE_EQ(result.count(), 1.5);
}

TEST(Duration, to_seconds_from_microseconds)
{
	auto duration = 2500000us;
	auto result = to_seconds(duration);
	EXPECT_DOUBLE_EQ(result.count(), 2.5);
}

TEST(Duration, to_seconds_from_nanoseconds)
{
	auto duration = 3500000000ns;
	auto result = to_seconds(duration);
	EXPECT_DOUBLE_EQ(result.count(), 3.5);
}

TEST(Duration, to_milliseconds_from_seconds)
{
	auto duration = std::chrono::seconds{2};
	auto result = to_milliseconds(duration);
	EXPECT_EQ(result.count(), 2000);
}

TEST(Duration, to_milliseconds_from_microseconds)
{
	auto duration = 5000us;
	auto result = to_milliseconds(duration);
	EXPECT_EQ(result.count(), 5);
}

TEST(Duration, to_microseconds_from_milliseconds)
{
	auto duration = 3ms;
	auto result = to_microseconds(duration);
	EXPECT_EQ(result.count(), 3000);
}

TEST(Duration, to_microseconds_from_nanoseconds)
{
	auto duration = 4000ns;
	auto result = to_microseconds(duration);
	EXPECT_EQ(result.count(), 4);
}

TEST(Duration, to_nanoseconds_from_microseconds)
{
	auto duration = 7us;
	auto result = to_nanoseconds(duration);
	EXPECT_EQ(result.count(), 7000);
}

TEST(Duration, to_nanoseconds_from_milliseconds)
{
	auto duration = 2ms;
	auto result = to_nanoseconds(duration);
	EXPECT_EQ(result.count(), 2000000);
}

TEST(Duration, precision_test_fractional_seconds)
{
	// Test conversion from milliseconds to fractional seconds
	auto duration = 123ms;
	auto result = to_seconds(duration);
	EXPECT_DOUBLE_EQ(result.count(), 0.123);
}

TEST(Duration, precision_test_small_values)
{
	// Test very small duration values
	auto duration = 1ns;
	auto result = to_seconds(duration);
	EXPECT_GT(result.count(), 0.0);
}

TEST(Duration, zero_duration)
{
	// Test zero duration conversion
	auto duration = std::chrono::steady_clock::duration::zero();
	
	EXPECT_DOUBLE_EQ(to_seconds(duration).count(), 0.0);
	EXPECT_EQ(to_milliseconds(duration).count(), 0);
	EXPECT_EQ(to_microseconds(duration).count(), 0);
	EXPECT_EQ(to_nanoseconds(duration).count(), 0);
}

TEST(Duration, constexpr_evaluation)
{
	// Test that functions can be evaluated at compile time
	constexpr auto duration = std::chrono::milliseconds{1000};
	constexpr auto result_ms = to_milliseconds(duration);
	constexpr auto result_us = to_microseconds(duration);
	constexpr auto result_ns = to_nanoseconds(duration);
	
	static_assert(result_ms.count() == 1000);
	static_assert(result_us.count() == 1000000);
	static_assert(result_ns.count() == 1000000000);
}

TEST(Duration, arithmetic_with_seconds_type)
{
	// Test that the seconds type works with arithmetic operations
	seconds s1{1.5};
	seconds s2{2.5};
	
	auto sum = s1 + s2;
	EXPECT_DOUBLE_EQ(sum.count(), 4.0);
	
	auto diff = s2 - s1;
	EXPECT_DOUBLE_EQ(diff.count(), 1.0);
}
