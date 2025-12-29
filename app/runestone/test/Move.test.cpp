#include <gtest/gtest.h>

import runestone.move;
import runestone.chessboard;

TEST(Move, constructor)
{
	constexpr runestone::Move move;

	EXPECT_EQ(move.from(), runestone::ChessBoard::Square::A1);
	EXPECT_EQ(move.to(), runestone::ChessBoard::Square::A1);
	EXPECT_EQ(move.type(), runestone::Move::Type::Normal);
	EXPECT_EQ(move.raw(), 0);
}

// TEST(Move, basicRoundTrip)
// {
// 	constexpr auto move = runestone::Move::make({runestone::ChessBoard::Square::E2, runestone::ChessBoard::Square::E4}, runestone::Move::Type::Normal, runestone::Move::Promo::Queen);
//
// 	EXPECT_EQ()
// }