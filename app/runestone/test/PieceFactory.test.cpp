#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>
#include <set>

import runestone.piecefactory;

namespace
{
	constexpr std::array<runestone::PieceFactory::Piece, runestone::PieceFactory::to_index<runestone::PieceFactory::Piece>()> WhitePieces = {
		runestone::PieceFactory::Piece::WhitePawn, runestone::PieceFactory::Piece::WhiteKnight, runestone::PieceFactory::Piece::WhiteBishop,
		runestone::PieceFactory::Piece::WhiteRook, runestone::PieceFactory::Piece::WhiteQueen,	runestone::PieceFactory::Piece::WhiteKing};

	constexpr std::array<runestone::PieceFactory::Piece, runestone::PieceFactory::to_index<runestone::PieceFactory::Piece>()> BlackPieces = {
		runestone::PieceFactory::Piece::BlackPawn, runestone::PieceFactory::Piece::BlackKnight, runestone::PieceFactory::Piece::BlackBishop,
		runestone::PieceFactory::Piece::BlackRook, runestone::PieceFactory::Piece::BlackQueen,	runestone::PieceFactory::Piece::BlackQueen};
}

TEST(PieceFactory, makePiece)
{
	std::set<runestone::PieceFactory::Piece> pieces;

	for (const auto type : magic_enum::enum_values<runestone::PieceFactory::Type>())
	{
		for (const auto color : magic_enum::enum_values<runestone::PieceFactory::Color>())
		{
			if (type == runestone::PieceFactory::Type::Size || color == runestone::PieceFactory::Color::Size)
			{
				EXPECT_EQ(runestone::PieceFactory::make_piece(type, color).error(), runestone::PieceFactory::Error::InvalidTypeOrColor);
				continue;
			}

			pieces.insert(runestone::PieceFactory::make_piece(type, color).value());
		}
	}

	for (const auto piece : magic_enum::enum_values<runestone::PieceFactory::Piece>())
	{
		if (piece == runestone::PieceFactory::Piece::Size)
		{
			EXPECT_FALSE(pieces.contains(piece)) << "Should not find piece: " << magic_enum::enum_name(piece);
			continue;
		}

		EXPECT_TRUE(pieces.contains(piece)) << "Could not find piece: " << magic_enum::enum_name(piece);
	}

	EXPECT_EQ(pieces.size(), runestone::PieceFactory::to_index<runestone::PieceFactory::Type>()
								 * runestone::PieceFactory::to_index<runestone::PieceFactory::Color>());
}

TEST(PieceFactory, pieceType)
{
	std::set<runestone::PieceFactory::Type> types;
	for (const auto type : magic_enum::enum_values<runestone::PieceFactory::Type>())
	{
		types.insert(type);
	}

	for (const auto piece : magic_enum::enum_values<runestone::PieceFactory::Piece>())
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::piece_type(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::piece_type(piece).has_value());
		EXPECT_TRUE(types.contains(runestone::PieceFactory::piece_type(piece).value()));
	}
}

TEST(PieceFactory, pieceColor)
{
	std::set<runestone::PieceFactory::Color> colors;
	for (const auto color : magic_enum::enum_values<runestone::PieceFactory::Color>())
	{
		colors.insert(color);
	}

	for (const auto piece : magic_enum::enum_values<runestone::PieceFactory::Piece>())
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::piece_color(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::piece_color(piece).has_value());
		EXPECT_TRUE(colors.contains(runestone::PieceFactory::piece_color(piece).value()));
	}
}

// When expanding from the macro TEST from GTEST the cognitive complexity
// greatly increase (48 > 25...project max). This check is suppressed as
// the code is readable and the actual cognitive complexity of this test is 8
// 2x for loops (+2), 2x if nested (+4), 2x control flow - continue (+2)
// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(PieceFactory, isWhite)
{
	for (const auto piece : WhitePieces)
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::white(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::white(piece).has_value());
		EXPECT_TRUE(runestone::PieceFactory::white(piece).value());
	}

	for (const auto piece : BlackPieces)
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::white(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::white(piece).has_value());
		EXPECT_FALSE(runestone::PieceFactory::white(piece).value());
	}
}
// NOLINTEND

// When expanding from the macro TEST from GTEST the cognitive complexity
// greatly increase (48 > 25...project max). This check is suppressed as
// the code is readable and the actual cognitive complexity of this test is 8
// 2x for loops (+2), 2x if nested (+4), 2x control flow - continue (+2)
// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(PieceFactory, isBlack)
{
	for (const auto piece : BlackPieces)
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::black(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::black(piece).has_value());
		EXPECT_TRUE(runestone::PieceFactory::black(piece).value());
	}

	for (const auto piece : WhitePieces)
	{
		if (piece == runestone::PieceFactory::Piece::Size || piece == runestone::PieceFactory::Piece::Empty)
		{
			EXPECT_EQ(runestone::PieceFactory::black(piece).error(), runestone::PieceFactory::Error::InvalidPiece);
			continue;
		}

		EXPECT_TRUE(runestone::PieceFactory::black(piece).has_value());
		EXPECT_FALSE(runestone::PieceFactory::black(piece).value());
	}
}
// NOLINTEND

TEST(PieceFactory, charToPieceWhitePieces)
{
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('P').value(), runestone::PieceFactory::Piece::WhitePawn);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('N').value(), runestone::PieceFactory::Piece::WhiteKnight);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('B').value(), runestone::PieceFactory::Piece::WhiteBishop);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('R').value(), runestone::PieceFactory::Piece::WhiteRook);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('Q').value(), runestone::PieceFactory::Piece::WhiteQueen);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('K').value(), runestone::PieceFactory::Piece::WhiteKing);
}

TEST(PieceFactory, charToPieceBlackPieces)
{
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('p').value(), runestone::PieceFactory::Piece::BlackPawn);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('n').value(), runestone::PieceFactory::Piece::BlackKnight);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('b').value(), runestone::PieceFactory::Piece::BlackBishop);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('r').value(), runestone::PieceFactory::Piece::BlackRook);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('q').value(), runestone::PieceFactory::Piece::BlackQueen);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('k').value(), runestone::PieceFactory::Piece::BlackKing);
}

TEST(PieceFactory, charToPieceInvalidCharacters)
{
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('x').error(), runestone::PieceFactory::Error::InvalidCharacter);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('1').error(), runestone::PieceFactory::Error::InvalidCharacter);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece(' ').error(), runestone::PieceFactory::Error::InvalidCharacter);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('\0').error(), runestone::PieceFactory::Error::InvalidCharacter);
	EXPECT_EQ(runestone::PieceFactory::char_to_piece('Z').error(), runestone::PieceFactory::Error::InvalidCharacter);
}

TEST(PieceFactory, pieceToCharWhitePieces)
{
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhitePawn).value(), 'P');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhiteKnight).value(), 'N');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhiteBishop).value(), 'B');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhiteRook).value(), 'R');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhiteQueen).value(), 'Q');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::WhiteKing).value(), 'K');
}

TEST(PieceFactory, pieceToCharBlackPieces)
{
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackPawn).value(), 'p');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackKnight).value(), 'n');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackBishop).value(), 'b');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackRook).value(), 'r');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackQueen).value(), 'q');
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::BlackKing).value(), 'k');
}

TEST(PieceFactory, pieceToCharInvalidPieces)
{
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::Empty).error(), runestone::PieceFactory::Error::InvalidPiece);
	EXPECT_EQ(runestone::PieceFactory::piece_to_char(runestone::PieceFactory::Piece::Size).error(), runestone::PieceFactory::Error::InvalidPiece);
}

TEST(PieceFactory, roundTripCharToPieceToChar)
{
	for (const std::string& valid_chars = "PNBRQKpnbrqk"; const auto c : valid_chars)
	{
		const auto piece = runestone::PieceFactory::char_to_piece(c);
		const auto result = runestone::PieceFactory::piece_to_char(piece.value());
		EXPECT_EQ(result, c) << "Round-trip failed for character: " << c;
	}
}

TEST(PieceFactory, roundTripPieceToCharToPiece)
{
	for (const auto piece : magic_enum::enum_values<runestone::PieceFactory::Piece>())
	{
		if (piece == runestone::PieceFactory::Piece::Empty || piece == runestone::PieceFactory::Piece::Size)
		{
			continue;
		}

		const auto c = runestone::PieceFactory::piece_to_char(piece);
		const auto result = runestone::PieceFactory::char_to_piece(c.value());
		EXPECT_EQ(result, piece) << "Round-trip failed for piece: " << magic_enum::enum_name(piece);
	}
}