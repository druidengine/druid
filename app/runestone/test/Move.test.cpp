#include <gtest/gtest.h>
#include <glm/fwd.hpp>
#include <magic_enum/magic_enum.hpp>

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

TEST(Move, specialTypes)
{
	constexpr auto ep = runestone::Move::make({.from = runestone::ChessBoard::Square::E5, .to = runestone::ChessBoard::Square::D6},
											  runestone::Move::Type::EnPassant, runestone::Move::Promo::Knight);
	EXPECT_EQ(ep.type(), runestone::Move::Type::EnPassant);

	constexpr auto castle = runestone::Move::make({.from = runestone::ChessBoard::Square::E1, .to = runestone::ChessBoard::Square::G1},
												  runestone::Move::Type::Castling, runestone::Move::Promo::Knight);
	EXPECT_EQ(castle.type(), runestone::Move::Type::Castling);
}

TEST(Move, rawBitLayout)
{
	constexpr auto move = runestone::Move::make({.from = runestone::ChessBoard::Square::E2, .to = runestone::ChessBoard::Square::E4},
												runestone::Move::Type::Normal, runestone::Move::Promo::Queen);

	constexpr auto raw = move.raw();

	static constexpr std::uint16_t square_mask{0x3F};
	static constexpr std::uint16_t two_bit_mask{0x3};
	static constexpr auto from_shift{6};
	static constexpr auto promo_shift{12};
	static constexpr auto type_shift{14};

	// Move.To
	EXPECT_EQ(raw & square_mask, static_cast<glm::u16>(runestone::ChessBoard::Square::E4));
	// Move.From
	EXPECT_EQ(raw >> from_shift & square_mask, static_cast<glm::u16>(runestone::ChessBoard::Square::E2));
	// Move.Promo
	EXPECT_EQ(raw >> promo_shift & two_bit_mask, static_cast<glm::u16>(runestone::Move::Promo::Queen));
	// Move.Type
	EXPECT_EQ(raw >> type_shift & two_bit_mask, static_cast<glm::u16>(runestone::Move::Type::Normal));
}

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(Move, exhaustiveFromToRoundTrip)
{
	for (auto from : magic_enum::enum_values<runestone::ChessBoard::Square>())
	{
		if (from == runestone::ChessBoard::Square::Size)
		{
			continue;
		}

		for (auto to : magic_enum::enum_values<runestone::ChessBoard::Square>())
		{
			if (to == runestone::ChessBoard::Square::Size)
			{
				continue;
			}

			const auto move = runestone::Move::make({.from = from, .to = to}, runestone::Move::Type::Normal, runestone::Move::Promo::Queen);

			EXPECT_EQ(move.from(), from);
			EXPECT_EQ(move.to(), to);
			EXPECT_EQ(move.type(), runestone::Move::Type::Normal);
		}
	}
}
// NOLINTEND