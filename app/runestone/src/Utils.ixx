/// @file Utils.ixx
/// @module runestone.utils
/// @brief Utility type traits and constexpr helpers for compile-time metaprogramming.
/// @details
/// This module provides general-purpose utilities used throughout the Runestone engine,
/// including:
/// - **Trait detection** for enumerations that define a sentinel `Size` member.
/// - **Constexpr utilities** such as `ToIndex()` for compile-time extraction of enum size values.
///
/// These helpers enable safer, strongly-typed patterns for metaprogramming and template logic.
module;

#include <type_traits>
#include <cstdint>

export module runestone.utils;

export namespace runestone::utils
{
	/// @brief Type trait to determine whether an enumeration defines a member named `Size`.
	/// @tparam T The type to inspect.
	/// @tparam U SFINAE parameter used for substitution detection (do not specify manually).
	template <typename T, typename U = void>
	struct HasSizeMember : std::false_type
	{
	};

	/// @brief Specialization of `HasSizeMember` for types that define a member named `Size`.
	/// @tparam T The type to inspect.
	template <typename T>
	struct HasSizeMember<T, std::void_t<decltype(T::Size)>> : std::true_type
	{
	};

	/// @brief Retrieve the size value from an enumeration that defines a `Size` member.
	/// @tparam T Enumeration type.
	/// @return The value of `T::Size` as a std::size_t.
	/// @note This function will fail to compile if `T` does not define `Size`.
	template <typename T>
	[[nodiscard]] consteval auto ToIndex() noexcept -> std::size_t
	{
		static_assert(std::is_enum_v<T>, "to_index<T> requires an enumeration type");
		static_assert(HasSizeMember<T>::value, "Enum type passed to to_index<T>() must define a member enumerator named 'Size'");

		return static_cast<std::size_t>(T::Size);
	}
}