#pragma once

#include <Types.h>

namespace runestone::utils
{
	/// @brief Convert FEN character to piece enum
	/// @param c Character representing a piece ('K'=white king, 'k'=black king, etc.)
	/// @return Corresponding Piece enum value, or Piece::Empty for invalid input
	constexpr auto CharToPiece(const char& c) -> Piece
	{
		switch (c)
		{
			case 'P':
			{
				return Piece::WhitePawn;
			}
			case 'B':
			{
				return Piece::WhiteBishop;
			}
			case 'N':
			{
				return Piece::WhiteKnight;
			}
			case 'R':
			{
				return Piece::WhiteRook;
			}
			case 'Q':
			{
				return Piece::WhiteQueen;
			}
			case 'K':
			{
				return Piece::WhiteKing;
			}
			case 'p':
			{
				return Piece::BlackPawn;
			}
			case 'b':
			{
				return Piece::BlackBishop;
			}
			case 'n':
			{
				return Piece::BlackKnight;
			}
			case 'r':
			{
				return Piece::BlackRook;
			}
			case 'q':
			{
				return Piece::BlackQueen;
			}
			case 'k':
			{
				return Piece::BlackKing;
			}
			default:
			{
				return Piece::Empty;
			}
		}
	}

	/// @brief Convert piece enum to FEN character
	/// @param piece Piece enum value to convert
	/// @return Corresponding FEN character ('K', 'k', 'P', 'p', etc.) or ' ' for invalid input
	constexpr auto PieceToChar(const Piece& piece) -> char
	{
		switch (piece)
		{
			case Piece::WhitePawn:
			{
				return 'P';
			}
			case Piece::WhiteBishop:
			{
				return 'B';
			}
			case Piece::WhiteKnight:
			{
				return 'N';
			}
			case Piece::WhiteRook:
			{
				return 'R';
			}
			case Piece::WhiteQueen:
			{
				return 'Q';
			}
			case Piece::WhiteKing:
			{
				return 'K';
			}
			case Piece::BlackPawn:
			{
				return 'p';
			}
			case Piece::BlackBishop:
			{
				return 'b';
			}
			case Piece::BlackKnight:
			{
				return 'n';
			}
			case Piece::BlackRook:
			{
				return 'r';
			}
			case Piece::BlackQueen:
			{
				return 'q';
			}
			case Piece::BlackKing:
			{
				return 'k';
			}
			default:
			{
				return ' ';
			}
		}
	}
}