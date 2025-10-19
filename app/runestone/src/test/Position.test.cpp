// #include <Position.h>
// #include <gtest/gtest.h>

// namespace
// {
// 	constexpr auto StartingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// 	constexpr auto StartingSideToMove = runestone::Color::White;
// 	constexpr auto StartingCastlingRights = runestone::CastlingRights::WhiteKingSide | runestone::CastlingRights::WhiteQueenSide
// 	| runestone::CastlingRights::BlackKingSide | runestone::CastlingRights::BlackQueenSide;
// 	constexpr auto StartingPositionEPSquare = runestone::Square::Size;
//
// }
//
// TEST(Position, setFromFenStartingPosition)
// {
// 	runestone::Position position;
// 	position.set_from_fen(StartingPosition);
//
// 	EXPECT_EQ(position.get_side_to_move(), StartingSideToMove);
// 	EXPECT_EQ(position.get_castling_rights(), StartingCastlingRights);
// 	EXPECT_EQ(position.get_en_passant_square(), StartingPositionEPSquare);
// 	EXPECT_EQ(position.get_half_move_clock(), 0);
// 	EXPECT_EQ(position.get_full_move_counter(), 1);
// }
