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

TEST(Move, basicRoundTrip)
{
	constexpr auto move = runestone::Move::make({.from = runestone::ChessBoard::Square::E2, .to = runestone::ChessBoard::Square::E4},
												runestone::Move::Type::Normal, runestone::Move::Promo::Queen);

	EXPECT_EQ(move.from(), runestone::ChessBoard::Square::E2);
	EXPECT_EQ(move.to(), runestone::ChessBoard::Square::E4);
	EXPECT_EQ(move.type(), runestone::Move::Type::Normal);
}

TEST(Move, promotionEncoding)
{
	constexpr auto move = runestone::Move::make({.from = runestone::ChessBoard::Square::A7, .to = runestone::ChessBoard::Square::A8},
												runestone::Move::Type::Promotion, runestone::Move::Promo::Queen);

	EXPECT_EQ(move.from(), runestone::ChessBoard::Square::A7);
	EXPECT_EQ(move.to(), runestone::ChessBoard::Square::A8);
	EXPECT_EQ(move.type(), runestone::Move::Type::Promotion);
	EXPECT_EQ(move.promo(), runestone::Move::Promo::Queen);
}