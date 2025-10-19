#include <BitBoard.h>
#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

namespace
{
	constexpr auto NumberOfSetBits8 = 8;
	constexpr auto NumberOfSetBits32 = NumberOfSetBits8 * 4;
	constexpr auto NumberOfSetBits64 = NumberOfSetBits32 * 2;

	/// @brief A single bit set inside a bitboard
	///
	/// @note The value was generated using the provided link https://gekomad.github.io/Cinnamon/BitboardCalculator/.
	/// After opening link, select layout 2 and check the square, A1, to generate hex value
	constexpr auto SingleSetBitBB = runestone::BitBoard{1};
	/// @brief Multiple bits set inside a bitboard
	///
	/// @note The value was generated using the provided link https://gekomad.github.io/Cinnamon/BitboardCalculator/.
	/// After opening link, select layout 2 and check the squares, A1, E6, and F7 to generate hex value
	constexpr auto MultipleSetBitsBB = runestone::BitBoard{0x20100000000001};

	constexpr auto Files = {runestone::bitboard::AFile, runestone::bitboard::BFile, runestone::bitboard::CFile, runestone::bitboard::DFile,
							runestone::bitboard::EFile, runestone::bitboard::FFile, runestone::bitboard::GFile, runestone::bitboard::HFile};
	constexpr auto Ranks = {runestone::bitboard::Rank1, runestone::bitboard::Rank2, runestone::bitboard::Rank3, runestone::bitboard::Rank4,
							runestone::bitboard::Rank5, runestone::bitboard::Rank6, runestone::bitboard::Rank7, runestone::bitboard::Rank8};
}

TEST(BitBoard, fileConstantBitBoards)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::AFile), NumberOfSetBits8);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::HFile), NumberOfSetBits8);

	// test that files do not overlap
	EXPECT_EQ(runestone::bitboard::AFile & runestone::bitboard::BFile, runestone::bitboard::EmptyBitBoard);
	EXPECT_EQ(runestone::bitboard::GFile & runestone::bitboard::HFile, runestone::bitboard::EmptyBitBoard);

	// test union of all files equals full bitboard
	auto all_files = runestone::bitboard::EmptyBitBoard;
	for (const auto file : Files)
	{
		all_files |= file;
	}
	EXPECT_EQ(all_files, runestone::bitboard::FullBitBoard);
}

TEST(BitBoard, rankConstantBitBoards)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::Rank1), NumberOfSetBits8);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::Rank8), NumberOfSetBits8);

	// test that files do not overlap
	EXPECT_EQ(runestone::bitboard::Rank1 & runestone::bitboard::Rank2, runestone::bitboard::EmptyBitBoard);
	EXPECT_EQ(runestone::bitboard::Rank7 & runestone::bitboard::Rank8, runestone::bitboard::EmptyBitBoard);

	// test union of all files equals full bitboard
	auto all_ranks = runestone::bitboard::EmptyBitBoard;
	for (const auto rank : Ranks)
	{
		all_ranks |= rank;
	}
	EXPECT_EQ(all_ranks, runestone::bitboard::FullBitBoard);
}

TEST(BitBoard, specialConstantBitBoards)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::DiagonalA1H8), NumberOfSetBits8);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::AntiDiagonalH1A8), NumberOfSetBits8);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::LightSquares), NumberOfSetBits32);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::DarkSquares), NumberOfSetBits32);

	// test that light and dark squares do not overlap
	EXPECT_EQ(runestone::bitboard::LightSquares & runestone::bitboard::DarkSquares, runestone::bitboard::EmptyBitBoard);
	EXPECT_EQ(runestone::bitboard::DiagonalA1H8 & runestone::bitboard::AntiDiagonalH1A8, runestone::bitboard::EmptyBitBoard);
}

/// @brief Test SquareBitBoard function for corner squares
///
/// @note Second arguments of EXPECT_EQ checks were generated using the provided link https://gekomad.github.io/Cinnamon/BitboardCalculator/.
/// After opening link, select layout 2 and check the squares to generate hex values
/// - A1 = 0x0000000000000001ULL
/// - H1 = 0x0000000000000080ULL
/// - A8 = 0x0100000000000000ULL
/// - H8 = 0x8000000000000000ULL
TEST(BitBoard, squareBitBoardCornerSquares)
{
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::A1), 0x0000000000000001ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::H1), 0x0000000000000080ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::A8), 0x0100000000000000ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::H8), 0x8000000000000000ULL);
}

/// @brief Test SquareBitBoard function for center squares
///
/// @note Second arguments of EXPECT_EQ checks were generated using the provided link https://gekomad.github.io/Cinnamon/BitboardCalculator/.
/// After opening link, select layout 2 and check the squares to generate hex values
/// - A1 = 0x0000000008000000ULL
/// - H1 = 0x0000000010000000ULL
/// - A8 = 0x0000000800000000ULL
/// - H8 = 0x0000001000000000ULL
TEST(BitBoard, squareBitBoardCenterSquares)
{
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::D4), 0x0000000008000000ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::E4), 0x0000000010000000ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::D5), 0x0000000800000000ULL);
	EXPECT_EQ(runestone::bitboard::SquareBitBoard(runestone::Square::E5), 0x0000001000000000ULL);
}

TEST(BitBoard, squareIsSetForEmptyBitBoard)
{
	for (const auto square : magic_enum::enum_values<runestone::Square>())
	{
		EXPECT_FALSE(runestone::bitboard::SquareIsSet(runestone::bitboard::EmptyBitBoard, square));
	}
}

TEST(BitBoard, squareIsSetForSingleBit)
{
	for (const auto square : magic_enum::enum_values<runestone::Square>())
	{
		switch (square)
		{
			case runestone::Square::A1:
			{
				EXPECT_TRUE(runestone::bitboard::SquareIsSet(SingleSetBitBB, runestone::Square::A1));
				break;
			}
			case runestone::Square::Size:
			{
				continue;
			}
			default:
			{
				EXPECT_FALSE(runestone::bitboard::SquareIsSet(SingleSetBitBB, square))
					<< "Check Failed for Square: " << magic_enum::enum_name(square);
				break;
			}
		}
	}
}

// when expanding from the macro TEST from GTEST the cognitive complexity
// greatly increase (35 > 25...project max). this check is suppressed as
// the code is readable and the actual cognitive complexity of this test is 9
// loop (+1), switch (+1), case (5 * +1), nested switch (+1), control flow - continue (+1)
// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(BitBoard, squareIsSetForMultipleBits)
{
	for (const auto square : magic_enum::enum_values<runestone::Square>())
	{
		switch (square)
		{
			case runestone::Square::A1:
			{
				EXPECT_TRUE(runestone::bitboard::SquareIsSet(MultipleSetBitsBB, runestone::Square::A1));
				break;
			}
			case runestone::Square::E6:
			{
				EXPECT_TRUE(runestone::bitboard::SquareIsSet(MultipleSetBitsBB, runestone::Square::E6));
				break;
			}
			case runestone::Square::F7:
			{
				EXPECT_TRUE(runestone::bitboard::SquareIsSet(MultipleSetBitsBB, runestone::Square::F7));
				break;
			}
			case runestone::Square::Size:
			{
				continue;
			}
			default:
			{
				EXPECT_FALSE(runestone::bitboard::SquareIsSet(MultipleSetBitsBB, square))
					<< "Check Failed for Square: " << magic_enum::enum_name(square);
				break;
			}
		}
	}
}
// NOLINTEND

TEST(BitBoard, setSquareForEmptyBitBoard)
{
	auto bitboard = runestone::bitboard::EmptyBitBoard;

	runestone::bitboard::SetSquare(bitboard, runestone::Square::A5);

	EXPECT_EQ(bitboard, runestone::bitboard::SquareBitBoard(runestone::Square::A5));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A5));
}

TEST(BitBoard, setSquareAddToExisting)
{
	auto bitboard = SingleSetBitBB;

	runestone::bitboard::SetSquare(bitboard, runestone::Square::A5);

	EXPECT_EQ(bitboard, runestone::bitboard::SquareBitBoard(runestone::Square::A1) | runestone::bitboard::SquareBitBoard(runestone::Square::A5));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A5));
}

TEST(BitBoard, setSquareForAlreadySet)
{
	auto bitboard = SingleSetBitBB;
	const auto original_bitboard = bitboard;

	runestone::bitboard::SetSquare(bitboard, runestone::Square::A1);

	// should remain unchanged
	EXPECT_EQ(bitboard, original_bitboard);
}

TEST(BitBoard, clearSquareForSingleBit)
{
	auto bitboard = SingleSetBitBB;

	runestone::bitboard::ClearSquare(bitboard, runestone::Square::A1);

	EXPECT_EQ(bitboard, runestone::bitboard::EmptyBitBoard);
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
}

TEST(BitBoard, clearSquareForMultipleBits)
{
	auto bitboard = MultipleSetBitsBB;

	runestone::bitboard::ClearSquare(bitboard, runestone::Square::A1);

	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::E6));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::F7));
}

TEST(BitBoard, clearSquareAlreadyCleared)
{
	auto bitboard = SingleSetBitBB;
	const auto original_bitboard = bitboard;

	runestone::bitboard::ClearSquare(bitboard, runestone::Square::E4);

	EXPECT_EQ(bitboard, original_bitboard);
}

TEST(BitBoard, fileOfAllFiles)
{
	constexpr auto file0 = 0;
	constexpr auto file1 = 1;
	constexpr auto file2 = 2;
	constexpr auto file3 = 3;
	constexpr auto file4 = 4;
	constexpr auto file5 = 5;
	constexpr auto file6 = 6;
	constexpr auto file7 = 7;

	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::A1), file0);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::B1), file1);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::C1), file2);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::D1), file3);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::E1), file4);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::F1), file5);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::G1), file6);
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::H1), file7);
}

TEST(BitBoard, fileOfWithDifferentRanks)
{
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::A1), runestone::bitboard::FileOf(runestone::Square::A8));
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::E1), runestone::bitboard::FileOf(runestone::Square::E4));
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::E4), runestone::bitboard::FileOf(runestone::Square::E8));
	EXPECT_EQ(runestone::bitboard::FileOf(runestone::Square::H2), runestone::bitboard::FileOf(runestone::Square::H7));
}

TEST(BitBoard, rankOfAllRanks)
{
	constexpr auto rank0 = 0;
	constexpr auto rank1 = 1;
	constexpr auto rank2 = 2;
	constexpr auto rank3 = 3;
	constexpr auto rank4 = 4;
	constexpr auto rank5 = 5;
	constexpr auto rank6 = 6;
	constexpr auto rank7 = 7;

	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A1), rank0);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A2), rank1);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A3), rank2);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A4), rank3);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A5), rank4);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A6), rank5);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A7), rank6);
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A8), rank7);
}

TEST(BitBoard, rankOfWithDifferentFiles)
{
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::A1), runestone::bitboard::RankOf(runestone::Square::H1));
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::B4), runestone::bitboard::RankOf(runestone::Square::G4));
	EXPECT_EQ(runestone::bitboard::RankOf(runestone::Square::C8), runestone::bitboard::RankOf(runestone::Square::F8));
}

TEST(BitBoard, makeSquareForCornerSquares)
{
	constexpr auto zero = 0;
	constexpr auto seven = 7;

	EXPECT_EQ(runestone::bitboard::MakeSquare(zero, zero), runestone::Square::A1);
	EXPECT_EQ(runestone::bitboard::MakeSquare(seven, zero), runestone::Square::H1);
	EXPECT_EQ(runestone::bitboard::MakeSquare(zero, seven), runestone::Square::A8);
	EXPECT_EQ(runestone::bitboard::MakeSquare(seven, seven), runestone::Square::H8);
}

TEST(BitBoard, makeSquareForCenterSquares)
{
	constexpr auto three = 3;
	constexpr auto four = 4;

	EXPECT_EQ(runestone::bitboard::MakeSquare(three, three), runestone::Square::D4);
	EXPECT_EQ(runestone::bitboard::MakeSquare(four, three), runestone::Square::E4);
	EXPECT_EQ(runestone::bitboard::MakeSquare(three, four), runestone::Square::D5);
	EXPECT_EQ(runestone::bitboard::MakeSquare(four, four), runestone::Square::E5);
}

/// @brief Test MakeSquare function for ensuring FileOf/RankOf are inverses
TEST(BitBoard, makeSquareRoundTrip)
{
	for (const auto square : magic_enum::enum_values<runestone::Square>())
	{
		if (square == runestone::Square::Size)
		{
			continue;
		}

		const auto file = runestone::bitboard::FileOf(square);
		const auto rank = runestone::bitboard::RankOf(square);

		EXPECT_EQ(runestone::bitboard::MakeSquare(file, rank), square);
	}
}

TEST(BitBoard, popCountForEmptyBitBoard)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::EmptyBitBoard), 0);
}

TEST(BitBoard, popCountForFullBitBoard)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::FullBitBoard), NumberOfSetBits64);
}

TEST(BitBoard, popCountForSingleBit)
{
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::SquareBitBoard(runestone::Square::A1)), 1);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::SquareBitBoard(runestone::Square::E4)), 1);
	EXPECT_EQ(runestone::bitboard::PopCount(runestone::bitboard::SquareBitBoard(runestone::Square::H8)), 1);
}

TEST(BitBoard, popCountForMultipleBits)
{
	constexpr auto bitboard = MultipleSetBitsBB;
	constexpr auto number_of_set_bits = 3;

	EXPECT_EQ(runestone::bitboard::PopCount(bitboard), number_of_set_bits);
}

TEST(BitBoard, lsbForSingleBit)
{
	EXPECT_EQ(runestone::bitboard::LSB(runestone::bitboard::SquareBitBoard(runestone::Square::A1)), runestone::Square::A1);
	EXPECT_EQ(runestone::bitboard::LSB(runestone::bitboard::SquareBitBoard(runestone::Square::E4)), runestone::Square::E4);
	EXPECT_EQ(runestone::bitboard::LSB(runestone::bitboard::SquareBitBoard(runestone::Square::H8)), runestone::Square::H8);
}

TEST(BitBoard, lsbForMultipleBits)
{
	EXPECT_EQ(runestone::bitboard::LSB(MultipleSetBitsBB), runestone::Square::A1);
}

TEST(BitBoard, msbForSingleBit)
{
	EXPECT_EQ(runestone::bitboard::MSB(runestone::bitboard::SquareBitBoard(runestone::Square::A1)), runestone::Square::A1);
	EXPECT_EQ(runestone::bitboard::MSB(runestone::bitboard::SquareBitBoard(runestone::Square::E4)), runestone::Square::E4);
	EXPECT_EQ(runestone::bitboard::MSB(runestone::bitboard::SquareBitBoard(runestone::Square::H8)), runestone::Square::H8);
}

TEST(BitBoard, MSBMultipleBits)
{
	EXPECT_EQ(runestone::bitboard::MSB(MultipleSetBitsBB), runestone::Square::F7);
}

TEST(BitBoard, popLSBForSingleBit)
{
	auto bitboard = SingleSetBitBB;
	const auto popped_square = runestone::bitboard::PopLSB(bitboard);

	EXPECT_EQ(popped_square, runestone::Square::A1);
	EXPECT_EQ(bitboard, runestone::bitboard::EmptyBitBoard);
}

TEST(BitBoard, popLSBForMultipleBits)
{
	auto bitboard = MultipleSetBitsBB;

	auto popped_square = runestone::bitboard::PopLSB(bitboard);

	EXPECT_EQ(popped_square, runestone::Square::A1);
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::E6));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::F7));

	popped_square = runestone::bitboard::PopLSB(bitboard);

	EXPECT_EQ(popped_square, runestone::Square::E6);
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::E6));
	EXPECT_TRUE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::F7));

	popped_square = runestone::bitboard::PopLSB(bitboard);

	EXPECT_EQ(popped_square, runestone::Square::F7);
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::A1));
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::E6));
	EXPECT_FALSE(runestone::bitboard::SquareIsSet(bitboard, runestone::Square::F7));
	EXPECT_EQ(bitboard, runestone::bitboard::EmptyBitBoard);
}