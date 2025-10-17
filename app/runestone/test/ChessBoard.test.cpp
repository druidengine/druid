#include <gtest/gtest.h>

import runestone.chessboard;

TEST(ChessBoard, defaultConstructsToEmpty)
{
    constexpr runestone::ChessBoard board;

    EXPECT_TRUE(board.empty());
    EXPECT_EQ(board.occupancies(), 0ULL);
    EXPECT_EQ(board.size(), 0);
}

TEST(ChessBoard, constructsFromRawValue)
{
	constexpr auto set_bits = 8;
    constexpr runestone::ChessBoard board(runestone::ChessBoard::AFile);

    EXPECT_EQ(board.occupancies(), runestone::ChessBoard::AFile);
    EXPECT_FALSE(board.empty());
    EXPECT_EQ(board.size(), set_bits);
}

TEST(ChessBoard, occupySquare)
{
	runestone::ChessBoard board;
	board.occupy_square(runestone::ChessBoard::Square::A1);

	EXPECT_TRUE(board.occupied(runestone::ChessBoard::Square::A1));
	EXPECT_FALSE(board.empty());
	EXPECT_EQ(board.size(), 1);

	board.occupy_square(runestone::ChessBoard::Square::H8);
	EXPECT_TRUE(board.occupied(runestone::ChessBoard::Square::H8));

	constexpr auto count = 2;
	EXPECT_EQ(board.size(), count);
}

TEST(ChessBoard, vacateSquare)
{
	runestone::ChessBoard board(runestone::ChessBoard::AFile & runestone::ChessBoard::Rank1);
	board.vacate_square(runestone::ChessBoard::Square::A1);

	EXPECT_FALSE(board.occupied(runestone::ChessBoard::Square::A1));
	EXPECT_EQ(board.size(), 0);
}

TEST(ChessBoard, flipSquareOccupancy)
{
	runestone::ChessBoard board;
    board.flip_square_occupancy(runestone::ChessBoard::Square::A2);
	board.flip_square_occupancy(runestone::ChessBoard::Square::A3);

    EXPECT_TRUE(board.occupied(runestone::ChessBoard::Square::A2));
    EXPECT_TRUE(board.occupied(runestone::ChessBoard::Square::A3));

	constexpr auto count = 2;
    EXPECT_EQ(board.size(), count);
}

TEST(ChessBoard, findFirstOccupancy)
{
    constexpr runestone::ChessBoard board(runestone::ChessBoard::AFile);
    EXPECT_EQ(board.find_first_occupancy(), 0);
}

TEST(ChessBoard, findLastOccupancy)
{
    constexpr runestone::ChessBoard board(runestone::ChessBoard::AFile);
	constexpr auto set_bit_index = 56;

    EXPECT_EQ(board.find_last_occupancy(), set_bit_index);
}

TEST(ChessBoard, popFirstOccupancy)
{
	constexpr auto bits_set2 = 0b10010000ULL;
	constexpr auto bit_index2 = 4;
	constexpr auto bits_set1 = 0b10000000ULL;
	constexpr auto bit_index1 = 7;

    runestone::ChessBoard board(bits_set2);

    EXPECT_EQ(board.pop_first_occupancy(), bit_index2);
    EXPECT_EQ(board.occupancies(), bits_set1);

    EXPECT_EQ(board.pop_first_occupancy(), bit_index1);
    EXPECT_TRUE(board.empty());
}

TEST(ChessBoard, unionOccupancies)
{
    constexpr runestone::ChessBoard afile(runestone::ChessBoard::AFile);
    constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
    constexpr runestone::ChessBoard result = afile.union_occupancies(bfile);

    EXPECT_EQ(result.occupancies(), runestone::ChessBoard::AFile | runestone::ChessBoard::BFile);
	// afile should be unchanged.
    EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile);
	// bfile should be unchanged.
    EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, intersectionOccupancies)
{
	constexpr runestone::ChessBoard afile(runestone::ChessBoard::AFile);
	constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
	constexpr runestone::ChessBoard result = afile.intersect_occupancies(bfile);

	EXPECT_EQ(result.occupancies(), runestone::ChessBoard::AFile & runestone::ChessBoard::BFile);
	// afile should be unchanged.
	EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile);
	// bfile should be unchanged.
	EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, sdiffOccupancies)
{
	constexpr runestone::ChessBoard afile(runestone::ChessBoard::AFile);
	constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
	constexpr runestone::ChessBoard result = afile.sdiff_occupancies(bfile);

	EXPECT_EQ(result.occupancies(), runestone::ChessBoard::AFile ^ runestone::ChessBoard::BFile);
	// afile should be unchanged.
	EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile);
	// bfile should be unchanged.
	EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, flipOccupancies)
{
    constexpr runestone::ChessBoard afile(runestone::ChessBoard::AFile);
    constexpr runestone::ChessBoard result = afile.flip_occupancies();

    EXPECT_EQ(result.occupancies(), ~runestone::ChessBoard::AFile);
	// afile should be unchanged.
	EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile);
}

TEST(ChessBoard, unionAssign)
{
    runestone::ChessBoard afile(runestone::ChessBoard::AFile);
    constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
    afile.union_assign(bfile);

    EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile | runestone::ChessBoard::BFile);
	// bfile should be unchanged.
	EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, intersectionAssign)
{
	runestone::ChessBoard afile(runestone::ChessBoard::AFile);
	constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
	afile.intersection_assign(bfile);

	EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile & runestone::ChessBoard::BFile);
	// bfile should be unchanged.
	EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, sdiffAssign)
{
	runestone::ChessBoard afile(runestone::ChessBoard::AFile);
	constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
	afile.sdiff_assign(bfile);

	EXPECT_EQ(afile.occupancies(), runestone::ChessBoard::AFile ^ runestone::ChessBoard::BFile);
	// bfile should be unchanged.
	EXPECT_EQ(bfile.occupancies(), runestone::ChessBoard::BFile);
}

TEST(ChessBoard, equalityAndInequalityOperators)
{
    constexpr runestone::ChessBoard afile(runestone::ChessBoard::AFile);
	constexpr runestone::ChessBoard afile2(runestone::ChessBoard::AFile);
    constexpr runestone::ChessBoard bfile(runestone::ChessBoard::BFile);
    constexpr runestone::ChessBoard cfile(runestone::ChessBoard::CFile);

    EXPECT_TRUE(afile.equals(afile2));
    EXPECT_TRUE(afile.not_equals(bfile));
    EXPECT_FALSE(afile.not_equals(afile2));
    EXPECT_FALSE(bfile.equals(cfile));
}
