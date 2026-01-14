#include <gtest/gtest.h>
#include <algorithm>
#include <magic_enum/magic_enum.hpp>
#include <vector>

import runestone.chesspiece;

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(ChessPiece, constructor)
{
	std::vector<runestone::ChessPiece> pieces;

	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			pieces.emplace_back(color, type);
		}
	}

	EXPECT_EQ(pieces.size(), magic_enum::enum_count<runestone::ChessPiece::Type>() * magic_enum::enum_count<runestone::ChessPiece::Color>());

	for (auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			const auto found =
				std::ranges::any_of(pieces,
									[&](const runestone::ChessPiece& p)
									{
										auto c = p.color();
										if (!c)
										{
											return p.type() == runestone::ChessPiece::Type::Empty;
										}
										return c.value() == color && p.type() == type && type != runestone::ChessPiece::Type::Empty;
									});
			EXPECT_TRUE(found) << "Missing/invalid piece for color=" << magic_enum::enum_name(color) << " type=" << magic_enum::enum_name(type);
		}
	}

	runestone::ChessPiece white_empty{runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Empty};
	EXPECT_FALSE(white_empty.color().has_value());
}
// NOLINTEND

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(ChessPiece, pieceType)
{
	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			const auto chess_piece = runestone::ChessPiece(color, type);
			EXPECT_EQ(chess_piece.type(), type);
		}
	}
}
// NOLINTEND

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(ChessPiece, pieceColor)
{
	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			const auto chess_piece = runestone::ChessPiece(color, type);
			if (type == runestone::ChessPiece::Type::Empty)
			{
				auto c = chess_piece.color();
				if (!c)
				{
					EXPECT_EQ(chess_piece.color().error(), runestone::ChessPiece::Error::InvalidInputPiece);
				}
			}

			if (type != runestone::ChessPiece::Type::Empty)
			{
				EXPECT_EQ(chess_piece.color().value(), color);
			}
		}
	}
}
// NOLINTEND

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(ChessPiece, isBlack)
{
	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			const auto chess_piece = runestone::ChessPiece(color, type);

			// Check for white pieces.
			if (color == runestone::ChessPiece::Color::White && type != runestone::ChessPiece::Type::Empty)
			{
				EXPECT_FALSE(chess_piece.black().value());
			}

			// Check for black pieces.
			if (color == runestone::ChessPiece::Color::Black && type != runestone::ChessPiece::Type::Empty)
			{
				EXPECT_TRUE(chess_piece.black().value());
			}

			// Check for emtpy pieces.
			if (type == runestone::ChessPiece::Type::Empty)
			{
				EXPECT_FALSE(chess_piece.black().has_value());

				auto c = chess_piece.black();
				if (!c)
				{
					EXPECT_EQ(chess_piece.black().error(), runestone::ChessPiece::Error::InvalidInputPiece);
				}
			}
		}
	}
}
// NOLINTEND

// NOLINTBEGIN (readability-function-cognitive-complexity,-warnings-as-errors)
TEST(ChessPiece, isWhite)
{
	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			const auto chess_piece = runestone::ChessPiece(color, type);

			// Check for white pieces.
			if (color == runestone::ChessPiece::Color::White && type != runestone::ChessPiece::Type::Empty)
			{
				EXPECT_TRUE(chess_piece.white().value());
			}

			// Check for black pieces.
			if (color == runestone::ChessPiece::Color::Black && type != runestone::ChessPiece::Type::Empty)
			{
				EXPECT_FALSE(chess_piece.white().value());
			}

			// Check for emtpy pieces.
			if (type == runestone::ChessPiece::Type::Empty)
			{
				EXPECT_FALSE(chess_piece.white().has_value());

				auto c = chess_piece.white();
				if (!c)
				{
					EXPECT_EQ(chess_piece.white().error(), runestone::ChessPiece::Error::InvalidInputPiece);
				}
			}
		}
	}
}
// NOLINTEND

TEST(ChessPiece, charToPieceWhitePieces)
{
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('P').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Pawn));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('N').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Knight));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('B').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Bishop));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('R').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Rook));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('Q').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Queen));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('K').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::King));
}

TEST(ChessPiece, charToPieceBlackPieces)
{
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('p').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Pawn));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('n').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Knight));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('b').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Bishop));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('r').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Rook));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('q').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Queen));
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('k').value(),
			  runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::King));
}

TEST(ChessPiece, charToPieceInvalidCharacters)
{
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('x').error(), runestone::ChessPiece::Error::InvalidInputCharacter);
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('1').error(), runestone::ChessPiece::Error::InvalidInputCharacter);
	EXPECT_EQ(runestone::ChessPiece::char_to_piece(' ').error(), runestone::ChessPiece::Error::InvalidInputCharacter);
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('\0').error(), runestone::ChessPiece::Error::InvalidInputCharacter);
	EXPECT_EQ(runestone::ChessPiece::char_to_piece('Z').error(), runestone::ChessPiece::Error::InvalidInputCharacter);
}

TEST(ChessPiece, pieceToCharWhitePieces)
{
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Pawn)),
			  'P');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Knight)),
			  'N');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Bishop)),
			  'B');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Rook)),
			  'R');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Queen)),
			  'Q');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::King)),
			  'K');
}

TEST(ChessPiece, pieceToCharBlackPieces)
{
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Pawn)),
			  'p');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Knight)),
			  'n');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Bishop)),
			  'b');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Rook)),
			  'r');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Queen)),
			  'q');
	EXPECT_EQ(runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::King)),
			  'k');
}

TEST(ChessPiece, pieceToCharInvalidInputPieces)
{
	EXPECT_EQ(
		runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::Black, runestone::ChessPiece::Type::Empty)).error(),
		runestone::ChessPiece::Error::InvalidInputPiece);
	EXPECT_EQ(
		runestone::ChessPiece::piece_to_char(runestone::ChessPiece(runestone::ChessPiece::Color::White, runestone::ChessPiece::Type::Empty)).error(),
		runestone::ChessPiece::Error::InvalidInputPiece);
}

TEST(ChessPiece, roundTripCharToPieceToChar)
{
	for (const std::string& valid_chars = "PNBRQKpnbrqk"; const auto c : valid_chars)
	{
		const auto piece = runestone::ChessPiece::char_to_piece(c);
		const auto result = runestone::ChessPiece::piece_to_char(piece.value());
		EXPECT_EQ(result, c) << "Round-trip failed for character: " << c;
	}
}

TEST(ChessPiece, roundTripPieceToCharToPiece)
{
	std::vector<runestone::ChessPiece> pieces;

	for (const auto color : magic_enum::enum_values<runestone::ChessPiece::Color>())
	{
		for (const auto type : magic_enum::enum_values<runestone::ChessPiece::Type>())
		{
			if (type == runestone::ChessPiece::Type::Empty)
			{
				continue;
			}
			pieces.emplace_back(color, type);
		}
	}

	for (const auto piece : pieces)
	{
		const auto c = runestone::ChessPiece::piece_to_char(piece);
		const auto result = runestone::ChessPiece::char_to_piece(c.value());
		EXPECT_EQ(result.value(), piece) << "Round-trip failed for piece: " << c.value();
	}
}
