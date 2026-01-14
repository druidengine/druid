/// @file Move.ixx
/// @module runestone.move
module;

#include <cstdint>

export module runestone.move;

import runestone.chessboard;

export namespace runestone
{
	/// @class Move
	/// @brief Compact, immutable representation of a chess move packed into 16 bits.
	///
	/// `Move` encodes a move using a fixed 16-bit layout:
	///
	/// - bits 0..5   : destination square (`to`)   (6 bits)
	/// - bits 6..11  : source square (`from`)      (6 bits)
	/// - bits 12..13 : promotion piece (`promo`)   (2 bits)
	/// - bits 14..15 : move type (`type`)          (2 bits)
	///
	/// The move type determines how the engine should interpret the move:
	/// - `Type::Normal`     : ordinary move (including captures)
	/// - `Type::Promotion`  : pawn promotion; `promo()` specifies the promoted piece
	/// - `Type::EnPassant`  : en-passant capture
	/// - `Type::Castling`   : castling move
	///
	/// @note Promotion information is only meaningful when `type() == Type::Promotion`.
	class Move
	{
	public:
		/// @enum Type
		/// @brief High-level category of move semantics.
		///
		/// Stored in bits 14..15.
		enum class Type : std::uint8_t
		{
			/// @brief Represents an ordinary move (may include capture depending on board state).
			Normal = 0,
			/// @brief Represents a pawn promotion.
			Promotion = 1,
			/// @brief Represents an en-passant capture.
			EnPassant = 2,
			/// @brief Represents a castling move.
			Castling = 3,
		};

		/// @enum Promo
		/// @brief Promotion piece selection.
		///
		/// Stored in bits 12..13 and only meaningful when `Type::Promotion`.
		enum class Promo : std::uint8_t
		{
			/// @brief Represents promotion to a knight.
			Knight = 0,
			/// @brief Represents a promotion to a bishop.
			Bishop = 1,
			/// @brief Represents a promotion to a rook.
			Rook = 2,
			/// @brief Represents a promotion to a queen.
			Queen = 3,
		};

		/// @struct Moving
		/// @brief Source and destination squares describing the geometric move.
		struct Moving
		{
			/// @brief Represents the source square.
			ChessBoard::Square from;
			/// @brief Represents the destination square.
			ChessBoard::Square to;
		};

		/// @brief Construct a null/default move (all fields zeroed).
		///
		/// A default move typically corresponds to `from == A1` / `to == A1`
		/// depending on the underlying `ChessBoard::Square` enumeration.
		constexpr Move() noexcept = default;

		/// @brief Create a packed move from squares and optional type/promotion metadata.
		///
		/// @param moving The source and destination squares.
		/// @param type The move category (normal/promotion/en-passant/castling).
		/// @param promo Promotion piece selection (used only when `type == Type::Promotion`).
		/// @return A packed `Move` instance.
		static constexpr auto make(Moving moving, Type type = Type::Normal, Promo promo = Promo::Queen) noexcept -> Move
		{
			Move move;
			move.data_ = (static_cast<std::uint16_t>(moving.to) & SquareMask) | ((static_cast<std::uint16_t>(moving.from) & SquareMask) << FromShift)
						 | ((static_cast<std::uint16_t>(promo) & TwoBitMask) << PromoShift)
						 | ((static_cast<std::uint16_t>(type) & TwoBitMask) << TypeShift);

			return move;
		}

		/// @brief Get the source square.
		/// @return The `from` square encoded in the move.
		[[nodiscard]] constexpr auto from() const noexcept -> ChessBoard::Square
		{
			return static_cast<ChessBoard::Square>((data_ >> FromShift) & SquareMask);
		}

		/// @brief Get the destination square.
		/// @return The `to` square encoded in the move.
		[[nodiscard]] constexpr auto to() const noexcept -> ChessBoard::Square
		{
			return static_cast<ChessBoard::Square>(data_ & SquareMask);
		}

		/// @brief Get the move type.
		/// @return The move category encoded in the move.
		[[nodiscard]] constexpr auto type() const noexcept -> Type
		{
			return static_cast<Type>((data_ >> TypeShift) & TwoBitMask);
		}

		/// @brief Get the promotion piece.
		///
		/// @note Only meaningful when `type() == Type::Promotion`.
		///
		/// @return The promotion piece encoded in the move.
		[[nodiscard]] constexpr auto promo() const noexcept -> Promo
		{
			return static_cast<Promo>((data_ >> PromoShift) & TwoBitMask);
		}

		/// @brief Get the raw packed 16-bit representation.
		/// @return Raw 16-bit move encoding.
		[[nodiscard]] constexpr auto raw() const noexcept -> std::uint16_t
		{
			return data_;
		}

	private:
		/// @brief Mask for a 6-bit square value (0..63).
		static constexpr std::uint16_t SquareMask{0x3F};
		/// @brief Mask for a 2-bit field (0..3).
		static constexpr std::uint16_t TwoBitMask{0x3};
		/// @brief Bit shift of the `to` field (bits 0..5).
		static constexpr auto ToShift{0};
		/// @brief Bit shift of the `from` field (bits 6..11).
		static constexpr auto FromShift{6};
		/// @brief Bit shift of the `promo` field (bits 12..13).
		static constexpr auto PromoShift{12};
		/// @brief Bit shift of the `type` field (bits 14..15).
		static constexpr auto TypeShift{14};
		std::uint16_t data_{0};
	};
} // namespace runestone
