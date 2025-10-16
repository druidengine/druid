#pragma once

#include <cstdint>

namespace runestone
{
	/// @brief 64-bit unsigned integer representation a chess board
	/// Uses little-endian rank-file mapping (rank 1 at low bits, rank 8 high bits)
	///
	/// For more information on bitboards, see https://www.chessprogramming.org/Bitboards
	///
	/// @note Each bit corresponds to a square on a chess board (0-63)
	/// Bit 0 = A1, Bit 7 = H1, Bit 56 = A8, Bit 63 = H8
	using BitBoard = std::uint64_t;

	// bit layout for our piece encoding:
	// bit:     7  6  5  4  3  2  1  0
	// usage:   -  -  -  -  C  T  T  T
	//                      |  |--|--|
	//                      |  +----- piece type (3 bits: 0-6)
	//                      +-------- color (1 bit: 0=white, 1=black)
	/// @brief Represents the color of a chess piece or player
	///
	/// @note Color is stored in bit 3 (0x8)
	enum class Color : std::uint8_t
	{
		// clang-format off
		White = 0,		// binary: 0b0000 = bit 3 is 0
		Black = 8,		// binary: 0b1000 = bit 3 is 1
		Size = 2
		// clang-format on
	};

	/// @brief Represents the type of chess piece (color-agnostic)
	///
	/// @note Piece type stored in bits 0-2 (0x7)
	enum class PieceType : std::uint8_t
	{
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
	};

	/// @brief Represents a specific chess piece (combines color and type)
	///
	/// @note Piece = Color (bit 3) | PieceType (bits 0-2)
	enum class Piece : std::uint8_t
	{
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
	};

	/// @brief Piece encoding utilities for fast bitwise operations
	///
	/// @note This namespace contains high-performance functions for extracting piece
	/// information using bitwise operations instead of slow switch statements.
	/// All functions are constexpr and will be inlined for optimal performance.
	/// @relates Piece
	namespace piece_encoding
	{
		/// @brief Bit mask for extracting piece type (bits 0-2)
		constexpr static auto HexSeven = 0x7U;
		/// @brief Bit mask for extracting color (bit 3)
		constexpr static auto HexEight = 0x8U;

		// fast extraction functions using bit operations
		/// @brief Extract piece type using a bit masking
		/// @param piece The piece
		/// @return The piece type
		///
		/// @note this approach avoids branching
		/// @relates Piece
		constexpr auto GetPieceType(Piece piece) noexcept -> PieceType
		{
			return static_cast<PieceType>(static_cast<std::uint8_t>(piece) & HexSeven);
		}

		/// @brief Extract color using a bit masking
		/// @param piece The piece
		/// @return The piece color
		///
		/// @note this approach avoids branching
		/// @relates Piece
		constexpr auto GetPieceColor(Piece piece) noexcept -> Color
		{
			return static_cast<Color>(static_cast<std::uint8_t>(piece) & HexEight);
		}

		/// @brief Create a piece from type and color
		/// @param type The piece type
		/// @param color The piece color
		/// @return The combined piece
		/// @relates Piece
		constexpr auto MakePiece(PieceType type, Color color) noexcept -> Piece
		{
			return static_cast<Piece>(static_cast<std::uint8_t>(type) | static_cast<std::uint8_t>(color));
		}

		/// @brief Check if piece is white (bit 3 == 0)
		/// @param piece The piece
		/// @return True if white
		/// @relates Piece
		constexpr auto IsWhite(Piece piece) noexcept -> bool
		{
			return (static_cast<std::uint8_t>(piece) & HexEight) == 0 && piece != Piece::Empty;
		}

		/// @brief Check if piece is black (bit 3 == 1)
		/// @param piece The piece
		/// @return True if black
		/// @relates Piece
		constexpr auto IsBlack(Piece piece) noexcept -> bool
		{
			return (static_cast<std::uint8_t>(piece) & HexEight) != 0;
		}
	}

	/// @brief Represents a square on a chess board
	///
	/// Squares are numbered 0-63 using little-endian rank-file mapping:
	/// A1 =0 , B1 = 1, ..., H1 = 7, A2 = 8, ..., H2 = 15, ..., A8 = 56, ..., H8 = 63.
	/// Lower ranks use lower bit numbers (standard chess programming layout).
	enum class Square : std::uint8_t
	{
		// clang-format off
		A1, B1, C1, D1, E1, F1, G1, H1,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A8, B8, C8, D8, E8, F8, G8, H8,
		Size
		// clang-format on
	};

	/// @brief Represents castling rights using bit flags
	enum class CastlingRights : std::uint8_t
	{
		// clang-format off
		None = 0,
		WhiteKingSide = 1, WhiteQueenSide = 2,
		BlackKingSide = 4, BlackQueenSide = 8,
		All = 15
		// clang-format on
	};
}