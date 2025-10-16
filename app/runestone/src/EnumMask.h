#pragma once

#include <type_traits>

namespace runestone
{
	// forward declaration of EnumMask
	template <typename Enum>
	class EnumMask;

	// forward declare the free function operators
	template <typename Enum>
	constexpr auto operator|(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>;

	template <typename Enum>
	constexpr auto operator&(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>;

	template <typename Enum>
	constexpr auto operator^(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>;

	template <typename Enum>
	constexpr auto operator~(Enum rhs) noexcept -> EnumMask<Enum>;

	/// @brief A type-safe wrapper for performing bitwise operations on enum values
	///
	/// @tparam Enum The enum type to wrap. Must be an actual enum type.
	///
	/// EnumMask provides a safe way to use enums as bit flags without losing type safety.
	/// It supports all standard bitwise operations (OR, AND, XOR, NOT) and their compound
	/// assignment variants. All operations are constexpr and noexcept.
	///
	/// @par Example
	/// @code
	/// enum class Permissions
	/// {
	///     Read = 1 << 0,
	///     Write = 1 << 1,
	///     Execute = 1 << 2
	/// };
	///
	/// EnumMask<Permissions> mask = Permissions::Read | Permissions::Write;
	/// if (mask.test(Permissions::Read))
	/// {
	///     // Has read permission
	/// }
	/// @endcode
	template <typename Enum>
	class EnumMask
	{
		static_assert(std::is_enum_v<Enum>, "EnumMask<T>: T must be an enum type");

	public:
		/// @brief The underlying integral type of the enum
		using UnderlyingType = std::underlying_type_t<Enum>;

		/// @brief Default constructor. Initializes mask to zero.
		constexpr EnumMask() noexcept = default;

		/// @brief Construct from a single enum valu
		/// @param value The enum value to initialize with
		constexpr explicit EnumMask(Enum value) noexcept : value_(static_cast<UnderlyingType>(value))
		{
		}

		/// @brief Construct from raw underlying type value
		/// @param value The raw integral value to initialize with
		constexpr explicit EnumMask(UnderlyingType value) noexcept : value_(value)
		{
		}

		/// @brief Bitwise OR with an enum value
		/// @param rhs The enum value to OR with
		/// @return New EnumMask containing the result
		constexpr auto operator|(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ | static_cast<UnderlyingType>(rhs));
		}

		/// @brief Bitwise OR with another EnumMask
		/// @param rhs The EnumMask to OR with
		/// @return New EnumMask containing the result
		constexpr auto operator|(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ | rhs.value_);
		}

		/// @brief Bitwise AND with an enum value
		/// @param rhs The enum value to AND with
		/// @return New EnumMask containing the result
		constexpr auto operator&(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ & static_cast<UnderlyingType>(rhs));
		}

		/// @brief Bitwise AND with another EnumMask
		/// @param rhs The EnumMask to AND with
		/// @return New EnumMask containing the result
		constexpr auto operator&(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ & rhs.value_);
		}

		/// @brief Bitwise XOR with an enum value
		/// @param rhs The enum value to XOR with
		/// @return New EnumMask containing the result
		constexpr auto operator^(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ ^ static_cast<UnderlyingType>(rhs));
		}

		/// @brief Bitwise XOR with another EnumMask
		/// @param rhs The EnumMask to XOR with
		/// @return New EnumMask containing the result
		constexpr auto operator^(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ ^ rhs.value_);
		}

		/// @brief Bitwise NOT operation
		/// @return New EnumMask with all bits inverted
		constexpr auto operator~() const noexcept -> EnumMask
		{
			return EnumMask(~value_);
		}

		/// @brief Compound bitwise OR assignment with enum value
		/// @param rhs The enum value to OR with
		/// @return Reference to this EnumMask
		constexpr auto operator|=(Enum rhs) noexcept -> EnumMask&
		{
			value_ |= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		/// @brief Compound bitwise OR assignment with EnumMask
		/// @param rhs The EnumMask to OR with
		/// @return Reference to this EnumMask
		constexpr auto operator|=(EnumMask rhs) noexcept -> EnumMask&
		{
			value_ |= rhs.value_;
			return *this;
		}

		/// @brief Compound bitwise AND assignment with enum value
		/// @param rhs The enum value to AND with
		/// @return Reference to this EnumMask
		constexpr auto operator&=(Enum rhs) noexcept -> EnumMask&
		{
			value_ &= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		/// @brief Compound bitwise AND assignment with EnumMask
		/// @param rhs The EnumMask to AND with
		/// @return Reference to this EnumMask
		constexpr auto operator&=(EnumMask rhs) noexcept -> EnumMask&
		{
			value_ &= rhs.value_;
			return *this;
		}

		/// @brief Compound bitwise XOR assignment with enum value
		/// @param rhs The enum value to XOR with
		/// @return Reference to this EnumMask
		constexpr auto operator^=(Enum rhs) noexcept -> EnumMask&
		{
			value_ ^= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		/// @brief Compound bitwise XOR assignment with EnumMask
		/// @param rhs The EnumMask to XOR with
		/// @return Reference to this EnumMask
		constexpr auto operator^=(EnumMask rhs) noexcept -> EnumMask&
		{
			value_ ^= rhs.value_;
			return *this;
		}

		/// @brief Test if a specific flag is set
		/// @param flag The enum flag to test for
		/// @return true if the flag is set, false otherwise
		[[nodiscard]] constexpr auto test(Enum flag) const noexcept -> bool
		{
			return (value_ & static_cast<UnderlyingType>(flag)) != 0;
		}

		/// @brief Get the raw underlying integral value
		/// @return The underlying integral representation of the mask
		[[nodiscard]] constexpr auto raw() const noexcept -> UnderlyingType
		{
			return value_;
		}

	private:
		UnderlyingType value_{0};
	};

	/// @brief Bitwise OR operator for two enum values
	/// @tparam Enum The enum type
	/// @param lhs Left-hand side enum value
	/// @param rhs Right-hand side enum value
	/// @return EnumMask containing the result of the OR operation
	/// @relates EnumMask
	template <typename Enum>
	constexpr auto operator|(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) | static_cast<U>(rhs));
	}

	/// @brief Bitwise AND operator for two enum values
	/// @tparam Enum The enum type
	/// @param lhs Left-hand side enum value
	/// @param rhs Right-hand side enum value
	/// @return EnumMask containing the result of the AND operation
	/// @relates EnumMask
	template <typename Enum>
	constexpr auto operator&(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) & static_cast<U>(rhs));
	}

	/// @brief Bitwise XOR operator for two enum values
	/// @tparam Enum The enum type
	/// @param lhs Left-hand side enum value
	/// @param rhs Right-hand side enum value
	/// @return EnumMask containing the result of the XOR operation
	/// @relates EnumMask
	template <typename Enum>
	constexpr auto operator^(Enum lhs, Enum rhs) noexcept -> EnumMask<Enum>
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
	}

	/// @brief Bitwise NOT operator for an enum value
	/// @tparam Enum The enum type
	/// @param rhs The enum value to invert
	/// @return EnumMask containing the inverted bits
	/// @relates EnumMask
	template <typename Enum>
	constexpr auto operator~(Enum rhs) noexcept -> EnumMask<Enum>
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(~static_cast<U>(rhs));
	}
}