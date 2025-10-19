#include <Utils.h>
#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

TEST(Utils, charToPieceWhitePieces)
{
	EXPECT_EQ(runestone::utils::CharToPiece('P'), runestone::Piece::WhitePawn);
	EXPECT_EQ(runestone::utils::CharToPiece('N'), runestone::Piece::WhiteKnight);
	EXPECT_EQ(runestone::utils::CharToPiece('B'), runestone::Piece::WhiteBishop);
	EXPECT_EQ(runestone::utils::CharToPiece('R'), runestone::Piece::WhiteRook);
	EXPECT_EQ(runestone::utils::CharToPiece('Q'), runestone::Piece::WhiteQueen);
	EXPECT_EQ(runestone::utils::CharToPiece('K'), runestone::Piece::WhiteKing);
}

TEST(Utils, charToPieceBlackPieces)
{
	EXPECT_EQ(runestone::utils::CharToPiece('p'), runestone::Piece::BlackPawn);
	EXPECT_EQ(runestone::utils::CharToPiece('n'), runestone::Piece::BlackKnight);
	EXPECT_EQ(runestone::utils::CharToPiece('b'), runestone::Piece::BlackBishop);
	EXPECT_EQ(runestone::utils::CharToPiece('r'), runestone::Piece::BlackRook);
	EXPECT_EQ(runestone::utils::CharToPiece('q'), runestone::Piece::BlackQueen);
	EXPECT_EQ(runestone::utils::CharToPiece('k'), runestone::Piece::BlackKing);
}

TEST(Utils, charToPieceInvalidCharacters)
{
	EXPECT_EQ(runestone::utils::CharToPiece('x'), runestone::Piece::Empty);
	EXPECT_EQ(runestone::utils::CharToPiece('1'), runestone::Piece::Empty);
	EXPECT_EQ(runestone::utils::CharToPiece(' '), runestone::Piece::Empty);
	EXPECT_EQ(runestone::utils::CharToPiece('\0'), runestone::Piece::Empty);
	EXPECT_EQ(runestone::utils::CharToPiece('Z'), runestone::Piece::Empty);
}

TEST(Utils, pieceToCharWhitePieces)
{
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhitePawn), 'P');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhiteKnight), 'N');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhiteBishop), 'B');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhiteRook), 'R');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhiteQueen), 'Q');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::WhiteKing), 'K');
}

TEST(Utils, pieceToCharBlackPieces)
{
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackPawn), 'p');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackKnight), 'n');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackBishop), 'b');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackRook), 'r');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackQueen), 'q');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::BlackKing), 'k');
}

TEST(Utils, pieceToCharInvalidPieces)
{
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::Empty), ' ');
	EXPECT_EQ(runestone::utils::PieceToChar(runestone::Piece::Size), ' ');
}

TEST(Utils, roundTripCharToPieceToChar)
{
	for (const std::string& valid_chars = "PNBRQKpnbrqk"; const auto c : valid_chars)
	{
		const auto piece = runestone::utils::CharToPiece(c);
		const auto result = runestone::utils::PieceToChar(piece);
		EXPECT_EQ(result, c) << "Round-trip failed for character: " << c;
	}
}

TEST(Utils, roundTripPieceToCharToPiece)
{
	for (const auto piece : magic_enum::enum_values<runestone::Piece>())
	{
		if (piece == runestone::Piece::Empty || piece == runestone::Piece::Size)
		{
			continue;
		}

		const auto c = runestone::utils::PieceToChar(piece);
		const auto result = runestone::utils::CharToPiece(c);
		EXPECT_EQ(result, piece) << "Round-trip failed for piece: " << magic_enum::enum_name(piece);
	}
}