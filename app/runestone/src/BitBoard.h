#pragma once

#include <Types.h>
#include <bit>

namespace runestone::bitboard
{
	// Constant (file, rank, edge, special) bitboards for testing and manipulating
	// bitboards within a chess program. For more information, see square mapping considerations
	// https://www.chessprogramming.org/Square_Mapping_Considerations and see general setwise operations
	// https://www.chessprogramming.org/General_Setwise_Operations
	// file bitboards
	constexpr BitBoard AFile = 0x0101010101010101ULL;
	constexpr BitBoard BFile = AFile << 1U;
	constexpr BitBoard CFile = AFile << 2U;
	constexpr BitBoard DFile = AFile << 3U;
	constexpr BitBoard EFile = AFile << 4U;
	constexpr BitBoard FFile = AFile << 5U;
	constexpr BitBoard GFile = AFile << 6U;
	constexpr BitBoard HFile = AFile << 7U;

	// rank bitboards
	constexpr BitBoard Rank1 = 0xFFULL;
	constexpr BitBoard Rank2 = Rank1 << (8 * 1U);
	constexpr BitBoard Rank3 = Rank1 << (8 * 2U);
	constexpr BitBoard Rank4 = Rank1 << (8 * 3U);
	constexpr BitBoard Rank5 = Rank1 << (8 * 4U);
	constexpr BitBoard Rank6 = Rank1 << (8 * 5U);
	constexpr BitBoard Rank7 = Rank1 << (8 * 6U);
	constexpr BitBoard Rank8 = Rank1 << (8 * 7U);

	// edge bitboards (for move generation)
	constexpr BitBoard NotAFile = ~AFile;
	constexpr BitBoard NotHFile = ~HFile;
	constexpr BitBoard NotRank1 = ~Rank1;
	constexpr BitBoard NotRank8 = ~Rank8;

	// special bitboards
	constexpr BitBoard EmptyBitBoard = 0ULL;
	constexpr BitBoard FullBitBoard = 0xFFFFFFFFFFFFFFFFULL;
	constexpr BitBoard LightSquares = 0x55AA55AA55AA55AAULL;
	constexpr BitBoard DarkSquares = 0xAA55AA55AA55AA55ULL;
	constexpr BitBoard DiagonalA1H8 = 0x8040201008040201ULL;
	constexpr BitBoard AntiDiagonalH1A8 = 0x0102040810204080ULL;

	// utility functions (bit manipulation functions)
	/// @brief Get a bitboard with only the specified square set
	/// @param square The square
	/// @return BitBoard with one bit set
	///
	/// @note For more information, see link https://www.chessprogramming.org/General_Setwise_Operations
	/// section Bit By Square
	/// @relates BitBoard
	constexpr auto SquareBitBoard(Square square) -> BitBoard
	{
		return 1ULL << static_cast<BitBoard>(square);
	}

	/// @brief Check if a square is set in a bitboard
	/// @param bitboard The bitboard to test
	/// @param square The square
	/// @return True, if the square is occupied
	///
	/// @note For more information, see link https://www.chessprogramming.org/General_Setwise_Operations
	/// section Bit By Square
	/// @relates BitBoard
	constexpr auto SquareIsSet(BitBoard bitboard, Square square) -> bool
	{
		return (bitboard & SquareBitBoard(square)) != 0;
	}

	/// @brief Set a square in a bitboard
	/// @param bitboard Reference to a bitboard to modify
	/// @param square The square to set
	///
	/// @note For more information, see link https://www.chessprogramming.org/General_Setwise_Operations
	/// section Bit By Square
	/// @relates BitBoard
	constexpr auto SetSquare(BitBoard& bitboard, Square square) -> void
	{
		bitboard |= SquareBitBoard(square);
	}

	/// @brief Clear a square in a bitboard
	/// @param bitboard Reference to a bitboard to modify
	/// @param square The square to clear
	///
	/// @note For more information, see link https://www.chessprogramming.org/General_Setwise_Operations
	/// section Bit By Square
	/// @relates BitBoard
	constexpr auto ClearSquare(BitBoard& bitboard, Square square) -> void
	{
		bitboard &= ~SquareBitBoard(square);
	}

	/// @brief Get the file of a square (0-7,A-H)
	/// @param square The square
	/// @return File index (0=A, 1=B, ..., 7=H)
	///
	/// @relates BitBoard
	constexpr auto FileOf(Square square) -> std::uint8_t
	{
		constexpr auto file_of_seven = 7U;

		return static_cast<std::uint8_t>(square) & file_of_seven;
	}

	/// @brief Get the rank of a square (0-7,1-8)
	/// @param square The square
	/// @return Rank index (0=1, 1=2, ..., 7=8)
	///
	/// @relates BitBoard
	constexpr auto RankOf(Square square) -> std::uint8_t
	{
		constexpr auto rank_of_three = 3U;

		return static_cast<std::uint8_t>(square) >> rank_of_three;
	}

	/// @brief Create a square from a file and rank
	/// @param file File (0-7)
	/// @param rank Rank (0-7)
	/// @return The square
	///
	/// @relates BitBoard
	constexpr auto MakeSquare(const int file, const int rank) -> Square
	{
		constexpr auto make_square_eight = 8U;

		return static_cast<Square>((rank * make_square_eight) + file);
	}

	/// @brief Count the number of sets bits in a bitboard (population count)
	/// @param bitboard The bitboard
	/// @return Number of sets bits
	///
	/// @relates BitBoard
	constexpr auto PopCount(BitBoard bitboard) -> int
	{
		return std::popcount(bitboard);
	}

	/// @brief Finds the least significant bit (LSB) position
	/// @param bitboard The bitboard (must not be zero)
	/// @return Square of the LSB
	///
	/// @relates BitBoard
	constexpr auto LSB(BitBoard bitboard) -> Square
	{
		return static_cast<Square>(std::countr_zero(bitboard));
	}

	/// @brief Find the most significant bit (MSB) position
	/// @param bitboard The bitboard (must not be zero)
	/// @return Square of the MSB
	///
	/// @relates BitBoard
	constexpr auto MSB(BitBoard bitboard) -> Square
	{
		constexpr auto bits = static_cast<int>(Square::Size);
		return static_cast<Square>(bits - 1 - std::countl_zero(bitboard));
	}

	/// @brief Pop (remove and return) the LSB from a bitboard
	/// @param bitboard Reference to bitboard to modify
	/// @return Square that was removed
	///
	/// @relates BitBoard
	constexpr auto PopLSB(BitBoard& bitboard) -> Square
	{
		const Square square = LSB(bitboard);
		// clear the LSB
		bitboard &= bitboard - 1;
		return square;
	}
}