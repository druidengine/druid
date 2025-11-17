/// @file PieceFactory.ixx
/// @module runestone.piecefactory
module;

#include <cstdint>
#include <expected>

export module runestone.piecefactory;

export namespace runestone
{
	/// @class PieceFactory
	/// @brief Enumeration-based factory for creating, identifying, and converting chess pieces.
	///
	/// The `PieceFactory` class defines color-agnostic (`Type`), color-specific (`Color`),
	/// and combined (`Piece`) enumerations representing all standard chess pieces.
	/// It provides utilities to construct, query, and convert these values
	/// between FEN characters and internal representations.
	///
	/// @note Each piece is encoded as a compact 8-bit value for efficient board representation.
	///	This layout allows constant-time bitwise extraction of color and piece type
	/// using the `HexSeven` and `HexEight` masks.
	///
	/// @par Bit Layout
	/// @code
	/// // bit layout for our piece encoding:
	/// // bit:     7  6  5  4  3  2  1  0
	/// // usage:   -  -  -  -  C  T  T  T
	/// //                      |  |--|--|
	/// //                      |  +----- piece type (3 bits: 0–6)
	/// //                      +-------- color (1 bit: 0=white, 1=black)
	/// @endcode
	class PieceFactory
	{
	public:
		/// @enum Type
		/// @brief Represents the type of chess piece (color-agnostic).
		/// @note Piece type stored in bits 0-2 (0x7).
		enum class Type : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			// clang-format off
			Empty = 0,		// 0b000
			Pawn = 1,		// 0b001
			Knight = 2,		// 0b010
			Bishop = 3,		// 0b011
			Rook = 4,		// 0b100
			Queen = 5,		// 0b101
			King = 6,		// 0b110
			Size = 7
			// clang-format on
			/// @endcond
		};

		/// @enum Color
		/// @brief Represents the color of a chess piece or player.
		/// @note Color is stored in bit 3 (0x8).
		enum class Color : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			// clang-format off
			White = 0,		// binary: 0b0000 = bit 3 is 0
			Black = 8,		// binary: 0b1000 = bit 3 is 1
			Size = 2
			// clang-format on
			/// @endcond
		};

		/// @enum Piece
		/// @brief Represents a specific chess piece (combines color and type).
		/// @note Piece = Color (bit 3) | PieceType (bits 0-2).
		enum class Piece : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			// clang-format off
			Empty = 0,
			// white pieces (color = 0, so bit 3 = 0)
			WhitePawn = 1,		// 0b0001 = 0 (white) | 1 (pawn)
			WhiteKnight = 2,	// 0b0010 = 0 (white) | 2 (knight)
			WhiteBishop = 3,	// 0b0011 = 0 (white) | 3 (bishop)
			WhiteRook = 4,		// 0b0100 = 0 (white) | 4 (rook)
			WhiteQueen = 5,		// 0b0101 = 0 (white) | 5 (queen)
			WhiteKing = 6,		// 0b0110 = 0 (white) | 6 (king)
			// black pieces (color = 8, so bit 3 = 1)
			BlackPawn = 9,		// 0b1001 = 8 (black) | 1 (pawn)
			BlackKnight = 10,	// 0b1010 = 8 (black) | 2 (knight)
			BlackBishop = 11,	// 0b1011 = 8 (black) | 3 (bishop)
			BlackRook = 12,		// 0b1100 = 8 (black) | 4 (rook)
			BlackQueen = 13,	// 0b1101 = 8 (black) | 5 (queen)
			BlackKing = 14,		// 0b1110 = 8 (black) | 6 (king)
			Size = 15
			// clang-format on
			/// @endcond
		};

		/// @enum Error
		/// @brief Represents a specific error thrown when calling piece factory utility functions.
		enum class Error : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			InvalidCharacter,
			InvalidPiece,
			InvalidTypeOrColor
			/// @endcond
		};

		/// @brief Create a piece from a type and color.
		/// @param type The type to make the piece.
		/// @param color The color to make the piece.
		/// @return The chess piece created from the given color and type.
		/// If the input type or color is ::Size then return Error::InvalidTypeOrColor.
		static constexpr auto make_piece(Type type, Color color) noexcept -> std::expected<Piece, Error>
		{
			if (type == Type::Size || color == Color::Size)
			{
				return std::unexpected(Error::InvalidTypeOrColor);
			}
			return static_cast<Piece>(static_cast<std::uint8_t>(type) | static_cast<std::uint8_t>(color));
		}

		/// @brief Get the chess piece type.
		/// @param piece Chess piece type to extract.
		/// @return The chess piece type. If piece is empty, return Error::InvalidPiece.
		static constexpr auto piece_type(Piece piece) noexcept -> std::expected<Type, Error>
		{
			if (piece == Piece::Empty || piece == Piece::Size)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return static_cast<Type>(static_cast<std::uint8_t>(piece) & HexSeven);
		}

		/// @brief Get the chess piece color.
		/// @param piece Chess piece color to extract.
		/// @return The chess piece color. If piece is empty, return Error::InvalidPiece.
		static constexpr auto piece_color(Piece piece) noexcept -> std::expected<Color, Error>
		{
			if (piece == Piece::Empty || piece == Piece::Size)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return static_cast<Color>(static_cast<std::uint8_t>(piece) & HexEight);
		}

		/// @brief Check if piece is white.
		/// @param piece Chess piece to check.
		/// @return True if white, false otherwise. If piece is empty,
		/// return Error::InvalidPiece.
		static constexpr auto white(Piece piece) noexcept -> std::expected<bool, Error>
		{
			if (piece == Piece::Empty || piece == Piece::Size)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return (static_cast<std::uint8_t>(piece) & HexEight) == 0;
		}

		/// @brief Check if piece is black.
		/// @param piece Chess piece to check if piece is black.
		/// @return True if black, false otherwise. If piece is empty,
		/// return Error::InvalidPiece.
		static constexpr auto black(Piece piece) noexcept -> std::expected<bool, Error>
		{
			if (piece == Piece::Empty || piece == Piece::Size)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return (static_cast<std::uint8_t>(piece) & HexEight) != 0;
		}

		/// @brief Convert a character to chess piece.
		/// @param c Character representing a piece ('K'=white king, 'k'=black king, etc.).
		/// @return Corresponding Piece enum value, or Error::InvalidCharacter.
		static constexpr auto char_to_piece(char c) noexcept -> std::expected<Piece, Error>
		{
			switch (c)
			{
				case 'P':
				{
					return Piece::WhitePawn;
				}
				case 'B':
				{
					return Piece::WhiteBishop;
				}
				case 'N':
				{
					return Piece::WhiteKnight;
				}
				case 'R':
				{
					return Piece::WhiteRook;
				}
				case 'Q':
				{
					return Piece::WhiteQueen;
				}
				case 'K':
				{
					return Piece::WhiteKing;
				}
				case 'p':
				{
					return Piece::BlackPawn;
				}
				case 'b':
				{
					return Piece::BlackBishop;
				}
				case 'n':
				{
					return Piece::BlackKnight;
				}
				case 'r':
				{
					return Piece::BlackRook;
				}
				case 'q':
				{
					return Piece::BlackQueen;
				}
				case 'k':
				{
					return Piece::BlackKing;
				}
				default:
				{
					return std::unexpected(Error::InvalidCharacter);
				}
			}
		}

		/// @brief Convert piece enum to FEN character.
		/// @param piece Piece enum value to convert.
		/// @return Corresponding FEN character ('K', 'k', 'P', 'p', etc.) or Error::InvalidPiece.
		static constexpr auto piece_to_char(Piece piece) noexcept -> std::expected<char, Error>
		{
			switch (piece)
			{
				case Piece::WhitePawn:
				{
					return 'P';
				}
				case Piece::WhiteBishop:
				{
					return 'B';
				}
				case Piece::WhiteKnight:
				{
					return 'N';
				}
				case Piece::WhiteRook:
				{
					return 'R';
				}
				case Piece::WhiteQueen:
				{
					return 'Q';
				}
				case Piece::WhiteKing:
				{
					return 'K';
				}
				case Piece::BlackPawn:
				{
					return 'p';
				}
				case Piece::BlackBishop:
				{
					return 'b';
				}
				case Piece::BlackKnight:
				{
					return 'n';
				}
				case Piece::BlackRook:
				{
					return 'r';
				}
				case Piece::BlackQueen:
				{
					return 'q';
				}
				case Piece::BlackKing:
				{
					return 'k';
				}
				default:
				{
					return std::unexpected(Error::InvalidPiece);
				}
			}
		}

	private:
		/// @brief Bit mask for extracting piece type (bits 0-2).
		static constexpr auto HexSeven = 0x7U;
		/// @brief Bit mask for extracting color (bit 3).
		static constexpr auto HexEight = 0x8U;
	};
}