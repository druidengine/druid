/// @file ChessPiece.ixx
/// @module runestone.chesspiece
module;

#include <cstdint>
#include <expected>

export module runestone.chesspiece;

export namespace runestone
{
	/// @class ChessPiece
	/// @brief Compact, enum-backed representation of a chess piece with helpers for FEN conversion.
	///
	/// @details
	/// `ChessPiece` encodes a chess piece in a single byte for fast board operations and cache-friendly
	/// storage. The encoding is intentionally simple:
	///
	/// - **Type** (bits 0–2): one of {Empty(0), Pawn(1), Knight(2), Bishop(3), Rook(4), Queen(5), King(6)}
	/// - **Color** (bit 3): White(0) or Black(1)
	///
	/// This layout allows constant-time bitwise extraction of color and type and enables efficient
	/// table lookups (e.g., attack masks, material values) without branching.
	/// The class also provides utilities to convert to/from FEN characters.
	///
	/// @par Bit layout
	/// @code
	/// // bit:     7  6  5  4  3  2  1  0
	/// // usage:   -  -  -  -  C  T  T  T
	/// //                      |  |--|--|
	/// //                      |  +----- piece type (0..6; 0 = Empty)
	/// //                      +-------- color (0 = White, 1 = Black)
	/// @endcode
	///
	/// @par Invariants & semantics
	/// - `Type::Empty` is **colorless**. When constructed with `type == Type::Empty`, the piece is set
	///   to empty regardless of the `Color` argument.
	/// - Non-empty pieces always carry a valid color bit and a type in the range `[Pawn..King]`.
	/// - Functions that conceptually require a non-empty piece (e.g., asking for a color) will return
	///   `std::unexpected(Error::InvalidInputPiece)` when invoked on `Empty`.
	class ChessPiece
	{
	public:
		/// @enum Type
		/// @brief Represents the type of chess piece (color-agnostic).
		/// @note Piece type stored in bits 0-2 (0x7).
		enum class Type : std::uint8_t
		{
			/// @brief Represents an empty piece (no piece).
			/// @note the bit representation: 0b000.
			Empty = 0,
			/// @brief Represents a pawn chess piece.
			/// @note the bit representation: 0b001.
			Pawn = 1,
			/// @brief Represents a knight chess piece.
			/// @note the bit representation: 0b010.
			Knight = 2,
			/// @brief Represents a bishop chess piece.
			/// @note the bit representation: 0b011.
			Bishop = 3,
			/// @brief Represents a rook chess piece.
			/// @note the bit representation: 0b100.
			Rook = 4,
			/// @brief Represents a queen chess piece.
			/// @note the bit representation: 0b101.
			Queen = 5,
			/// @brief Represents a pawn chess piece.
			/// @note the bit representation: 0b110.
			King = 6
		};

		/// @enum Color
		/// @brief Represents the color of a chess piece or player.
		/// @note Color is stored in bit 3 (0x8).
		enum class Color : std::uint8_t
		{
			/// @brief Represents the white player.
			/// @note the bit representation: 0b0000 = bit 3 is 0.
			White = 0,
			/// @brief Represents the black player.
			/// @note the bit representation: 0b1000 = bit 3 is 1.
			Black = 8
		};

		/// @enum Error
		/// @brief Represents a specific error thrown when calling chess piece utility functions.
		enum class Error : std::uint8_t
		{
			/// @brief Error denoting that the input character is invalid.
			InvalidInputCharacter,
			/// @brief Error denoting that the input piece is invalid.
			InvalidInputPiece
		};

		/// @brief Convert a character to chess piece.
		/// @param c Character representing a piece ('K'=white king, 'k'=black king, etc.).
		/// @return Corresponding Piece enum value, or Error::InvalidCharacter.
		[[nodiscard]] static constexpr auto char_to_piece(char c) noexcept -> std::expected<ChessPiece, Error>
		{
			switch (c)
			{
				case 'P':
				{
					return ChessPiece(Color::White, Type::Pawn);
				}
				case 'B':
				{
					return ChessPiece(Color::White, Type::Bishop);
				}
				case 'N':
				{
					return ChessPiece(Color::White, Type::Knight);
				}
				case 'R':
				{
					return ChessPiece(Color::White, Type::Rook);
				}
				case 'Q':
				{
					return ChessPiece(Color::White, Type::Queen);
				}
				case 'K':
				{
					return ChessPiece(Color::White, Type::King);
				}
				case 'p':
				{
					return ChessPiece(Color::Black, Type::Pawn);
				}
				case 'b':
				{
					return ChessPiece(Color::Black, Type::Bishop);
				}
				case 'n':
				{
					return ChessPiece(Color::Black, Type::Knight);
				}
				case 'r':
				{
					return ChessPiece(Color::Black, Type::Rook);
				}
				case 'q':
				{
					return ChessPiece(Color::Black, Type::Queen);
				}
				case 'k':
				{
					return ChessPiece(Color::Black, Type::King);
				}
				default:
				{
					return std::unexpected(Error::InvalidInputCharacter);
				}
			}
		}

		/// @brief Convert chess piece enum to FEN character.
		/// @param chess_piece Piece enum value to convert.
		/// @return Corresponding FEN character ('K', 'k', 'P', 'p', etc.) or Error::InvalidInputPiece.
		[[nodiscard]] static constexpr auto piece_to_char(ChessPiece chess_piece) noexcept -> std::expected<char, Error>
		{
			switch (chess_piece.raw())
			{
				case UnderlyingType::WhitePawn:
				{
					return 'P';
				}
				case UnderlyingType::WhiteBishop:
				{
					return 'B';
				}
				case UnderlyingType::WhiteKnight:
				{
					return 'N';
				}
				case UnderlyingType::WhiteRook:
				{
					return 'R';
				}
				case UnderlyingType::WhiteQueen:
				{
					return 'Q';
				}
				case UnderlyingType::WhiteKing:
				{
					return 'K';
				}
				case UnderlyingType::BlackPawn:
				{
					return 'p';
				}
				case UnderlyingType::BlackBishop:
				{
					return 'b';
				}
				case UnderlyingType::BlackKnight:
				{
					return 'n';
				}
				case UnderlyingType::BlackRook:
				{
					return 'r';
				}
				case UnderlyingType::BlackQueen:
				{
					return 'q';
				}
				case UnderlyingType::BlackKing:
				{
					return 'k';
				}
				default:
				{
					return std::unexpected(Error::InvalidInputPiece);
				}
			}
		}

		/// @brief Default constructs a chess piece.
		explicit constexpr ChessPiece() noexcept = default;

		/// @brief Construct a chess piece from a color and type.
		///
		/// This constructor encodes the given `Color` and `Type` into the internal
		/// compact 8-bit representation. If `type == Type::Empty`, the piece is treated
		/// as colorless and the `color` parameter is ignored. Otherwise, the final value
		/// is computed as:
		///
		/// @code
		///   encoded = static_cast<uint8_t>(type) | static_cast<uint8_t>(color);
		/// @endcode
		///
		/// @param color The color of the piece (ignored when `type == Type::Empty`).
		/// @param type  The piece type. If `Type::Empty`, the resulting piece becomes
		///              `UnderlyingType::Empty` regardless of `color`.
		explicit constexpr ChessPiece(Color color, Type type) noexcept
		{
			if (type == Type::Empty)
			{
				underlying_type_ = UnderlyingType::Empty;
			}
			else
			{
				underlying_type_ = static_cast<UnderlyingType>(static_cast<uint8_t>(type) | static_cast<uint8_t>(color));
			}
		}

		/// @brief Compare two chess pieces for equality.
		///
		/// Two `ChessPiece` objects are considered equal if both their encoded
		/// underlying values and raw bit representations match exactly. This ensures
		/// that comparisons reflect both the piece type and its color.
		///
		/// @param chess_piece The other piece to compare with.
		/// @return `true` if both pieces represent the same color and type;
		///         `false` otherwise.
		[[nodiscard]] auto operator==(const ChessPiece& chess_piece) const -> bool
		{
			return raw() == chess_piece.raw() && raw_bits() == chess_piece.raw_bits();
		}

		/// @brief Get the chess piece type.
		/// @return The chess piece type.
		[[nodiscard]] constexpr auto type() const noexcept -> Type
		{
			return static_cast<Type>(raw_bits() & HexSeven);
		}

		/// @brief Get the chess piece color.
		/// @return The chess piece color.
		[[nodiscard]] constexpr auto color() const noexcept -> std::expected<Color, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidInputPiece);
			}
			return static_cast<Color>(raw_bits() & HexEight);
		}

		/// @brief Check if piece is white.
		/// @return True if white, false otherwise. If piece is empty,
		/// return Error::InvalidInputPiece.
		[[nodiscard]] constexpr auto white() const noexcept -> std::expected<bool, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidInputPiece);
			}
			return (raw_bits() & HexEight) == 0;
		}

		/// @brief Check if piece is black.
		/// @return True if black, false otherwise. If piece is empty,
		/// return Error::InvalidInputPiece.
		[[nodiscard]] constexpr auto black() const noexcept -> std::expected<bool, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidInputPiece);
			}
			return (raw_bits() & HexEight) != 0;
		}

	private:
		/// @enum UnderlyingType
		/// @brief Represents a specific chess piece (combines color and type).
		/// @note Piece = Color (bit 3) | Type (bits 0-2).
		enum class UnderlyingType : std::uint8_t
		{
			/// @brief Represents no chess piece.
			/// @note the bit representation: 0bX000
			Empty = 0,
			// white pieces (color = 0, so bit 3 = 0)
			/// @brief Represents a white pawn.
			/// @note the bit representation: 0b0001 = 0 (white) | 1 (pawn)
			WhitePawn = 1,
			/// @brief Represents a white knight.
			/// @note the bit representation: 0b0010 = 0 (white) | 2 (knight)
			WhiteKnight = 2,
			/// @brief Represents a white bishop.
			/// @note the bit representation: 0b0011 = 0 (white) | 3 (bishop)
			WhiteBishop = 3,
			/// @brief Represents a white rook.
			/// @note the bit representation: 0b0100 = 0 (white) | 4 (rook)
			WhiteRook = 4,
			/// @brief Represents a white queen.
			/// @note the bit representation: 0b0101 = 0 (white) | 5 (queen)
			WhiteQueen = 5,
			/// @brief Represents a white king.
			/// @note the bit representation: 0b0110 = 0 (white) | 6 (king)
			WhiteKing = 6,

			// black pieces (color = 8, so bit 3 = 1)
			/// @brief Represents a black pawn.
			/// @note the bit representation: 0b1001 = 8 (black) | 1 (pawn)
			BlackPawn = 9,
			/// @brief Represents a black knight.
			/// @note the bit representation: 0b1010 = 8 (black) | 2 (knight)
			BlackKnight = 10,
			/// @brief Represents a black bishop.
			/// @note the bit representation: 0b1011 = 8 (black) | 3 (bishop)
			BlackBishop = 11,
			/// @brief Represents a black rook.
			/// @note the bit representation: 0b1100 = 8 (black) | 4 (rook)
			BlackRook = 12,
			/// @brief Represents a black queen.
			/// @note the bit representation: 0b1101 = 8 (black) | 5 (queen)
			BlackQueen = 13,
			/// @brief Represents a black king.
			/// @note the bit representation: 0b1110 = 8 (black) | 6 (king)
			BlackKing = 14,
		};
		/// @brief Bit mask for extracting piece type (bits 0-2).
		static constexpr auto HexSeven{0x7U};
		/// @brief Bit mask for extracting color (bit 3).
		static constexpr auto HexEight{0x8U};
		UnderlyingType underlying_type_{UnderlyingType::Empty};

		/// @brief Get the underlying type of ChessPiece represented as bits.
		/// @return The underlying type as bits.
		[[nodiscard]] constexpr auto raw_bits() const noexcept -> std::uint8_t
		{
			return static_cast<std::uint8_t>(underlying_type_);
		}

		/// @brief Get the underlying type of ChessPiece.
		/// @return The underlying type.
		[[nodiscard]] constexpr auto raw() const noexcept -> UnderlyingType
		{
			return underlying_type_;
		}
	};
}
