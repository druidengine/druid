#include <BitBoard.h>
#include <Position.h>
#include <Utils.h>
#include <cctype>
#include <string>

using runestone::Position;

auto Position::set_from_fen(std::string_view fen_string) -> void
{
	clear_position();

	std::string file_rank;
	std::string half_move_counter;
	std::string full_move_counter;

	auto current_square_centric_index = 0;
	for (const auto c : fen_string)
	{
		// fen strings are space delimited. if a space is encountered,
		// move to next field in FenStringFields enum class
		if (c != ' ')
		{
			switch (current_fen_string_field_)
			{
				case FenStringFields::PiecePlacement:
				{
					if (c != '/')
					{
						create_piece(utils::CharToPiece(c), static_cast<Square>(current_square_centric_index));
						// if the character is a digit (i.e. the number of empty sequential squares), increment the current square centric index
						// by the value of the character. else increment by one (i.e. a piece was created)
						current_square_centric_index += (std::isdigit(c) == 1) ? c - '0' : 1;
					}
					break;
				}
				case FenStringFields::SideToMove:
				{
					set_side_to_move(c);
					break;
				}
				case FenStringFields::CastlingAbility:
				{
					set_castling_rights(c);
					break;
				}
				case FenStringFields::EnPassantTargetSquare:
				{
					file_rank += c;
					set_en_passant_square(file_rank);
					break;
				}
				case FenStringFields::HalfmoveClock:
				{
					half_move_counter += c;
					set_half_move_clock(half_move_counter);
					break;
				}
				case FenStringFields::FullmoveCounter:
				{
					full_move_counter += c;
					set_full_move_counter(full_move_counter);
					break;
				}
				default:
				{
				}
			}
		}
		else
		{
			current_fen_string_field_ = static_cast<FenStringFields>(static_cast<int>(current_fen_string_field_) + 1);
		}
	}
}

auto Position::create_piece(Piece piece, Square square) -> void
{
	// update the square-centric board
	// NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-constant-array-index)
	board_[static_cast<std::size_t>(square)] = piece;

	if (piece == Piece::Empty)
	{
		return;
	}

	const auto square_bitboard = bitboard::SquareBitBoard(square);

	// extract type and color using bit masks
	const auto piece_type = piece_encoding::GetPieceType(piece);
	const auto color = piece_encoding::GetPieceColor(piece);
	const auto color_index = static_cast<std::size_t>(color) >> 3U;

	// update bitboards maintaining color and piece
	// NOLINTBEGIN (cppcoreguidelines-pro-bounds-constant-array-index)
	bitboard_by_piece_[static_cast<std::size_t>(piece_type)] |= square_bitboard;
	bitboard_by_color_[color_index] |= square_bitboard;
	// NOLINTEND
}

auto Position::clear_position() -> void
{
	// reset all bitboards
	board_.fill(Piece::Empty);
	bitboard_by_piece_.fill(0ULL);
	bitboard_by_color_.fill(0ULL);

	// reset all game state variables
	half_move_clock_ = 0;
	full_move_number_ = 1;
	side_to_move_ = Color::White;
	castling_rights_ = CastlingRights{runestone::CastlingRights::None};
	enpassant_square_ = Square::Size;

	// update fen string field for parsing
	current_fen_string_field_ = FenStringFields::PiecePlacement;
}

auto Position::set_side_to_move(char c) -> void
{
	side_to_move_ = (c == 'w') ? Color::White : Color::Black;
}

auto Position::get_side_to_move() const noexcept -> Color
{
	return side_to_move_;
}

auto Position::set_castling_rights(char c) -> void
{
	switch (c)
	{
		case 'K':
		{
			castling_rights_ |= runestone::CastlingRights::WhiteKingSide;
			break;
		}
		case 'Q':
		{
			castling_rights_ |= runestone::CastlingRights::WhiteQueenSide;
			break;
		}
		case 'k':
		{
			castling_rights_ |= runestone::CastlingRights::BlackKingSide;
			break;
		}
		case 'q':
		{
			castling_rights_ |= runestone::CastlingRights::BlackQueenSide;
			break;
		}
		default:
		{
			castling_rights_ = CastlingRights{runestone::CastlingRights::None};
			break;
		}
	}
}

auto Position::get_castling_rights() const noexcept -> CastlingRights
{
	return castling_rights_;
}

auto Position::set_en_passant_square(const std::string& file_rank) -> void
{
	// a enpassant string must be two chars long
	// alpha, numeric (i.e. e6)
	if (file_rank.size() != 2)
	{
		// '-' is a valid (meaning no enpassant possible)
		if (file_rank == "-")
		{
			enpassant_square_ = Square::Size;
		}

		return;
	}

	const auto file = file_rank.at(0) - 'a';
	const auto rank = file_rank.at(1) - '1';
	constexpr auto eight = 8;
	enpassant_square_ = static_cast<Square>((rank * eight) + file);
}

auto Position::get_en_passant_square() const noexcept -> Square
{
	return enpassant_square_;
}

auto Position::set_half_move_clock(const std::string& counter) -> void
{
	half_move_clock_ = std::stoi(counter);
}

[[nodiscard]] auto Position::get_half_move_clock() const noexcept -> int
{
	return half_move_clock_;
}

auto Position::set_full_move_counter(const std::string& counter) -> void
{
	full_move_number_ = std::stoi(counter);
}

[[nodiscard]] auto Position::get_full_move_counter() const noexcept -> int
{
	return full_move_number_;
}
