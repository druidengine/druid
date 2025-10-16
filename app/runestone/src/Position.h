#pragma once

#include <EnumMask.h>
#include <Types.h>
#include <array>
#include <string>
#include <string_view>

namespace runestone
{
	/// @class Position
	/// @brief Represents a complete chess position with board state and game metadata
	///
	/// The Position class uses a hybrid board representation:
	/// - Square-centric array for fast "what piece is on square X?" queries
	/// - Piece-centric bitboards for fast "where are all white pawns?" queries
	/// - Color-centric bitboards for fast "where are all white pieces?" queries
	///
	/// Game state includes:
	/// - Side to move (White/Black)
	/// - Castling rights (KQkq flags)
	/// - En passant target square
	/// - Half-move clock (for 50-move rule)
	/// - Full-move number (increments after Black's move)
	///
	/// @note This class follows the chess programming convention of using
	/// Forsyth-Edwards Notation (FEN) for position setup.
	///
	/// For more information see, https://www.chessprogramming.org/Forsyth-Edwards_Notation
	/// and https://www.chessprogramming.org/Board_Representation
	class Position
	{
	public:
		/// @brief Type alias for castling rights bitmask
		///
		/// Combines multiple CastlingRights flags using bitwise operations.
		/// Typical values: None, WhiteKingside, WhiteQueenside, BlackKingside, BlackQueenside
		using CastlingRights = EnumMask<CastlingRights>;

		/// @enum FenStringFields
		/// @brief Enumeration of the 6 fields in a FEN string
		///
		/// FEN format: "pieces side castling ep halfmove fullmove"
		/// Example: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
		///
		/// Fore more information on FEN strings, see
		/// https://www.chessprogramming.org/Forsyth-Edwards_Notation
		enum class FenStringFields : std::uint8_t
		{
			/// @brief Field 1: Piece placement (rank 8 to rank 1)
			///
			/// Uses standard piece letters (PNBRQK/pnbrqk) and digits for empty squares.
			/// Ranks separated by '/'. Example: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
			PiecePlacement = 0,

			/// @brief Field 2: Active color ('w' or 'b')
			///
			/// Indicates which side is to move. 'w' = White, 'b' = Black
			SideToMove = 1,

			/// @brief Field 3: Castling availability (KQkq or -)
			///
			/// K = White kingside, Q = White queenside, k = Black kingside, q = Black queenside
			/// '-' indicates no castling rights available
			CastlingAbility = 2,

			/// @brief Field 4: En passant target square (algebraic notation or -)
			///
			/// The square behind a pawn that just moved two squares forward.
			/// Format: file-rank (e.g., "e3"). '-' if no en passant possible
			EnPassantTargetSquare = 3,

			/// @brief Field 5: Half-move clock (positive integer)
			///
			/// Number of half-moves since last capture or pawn advance.
			/// Used for the 50-move rule (draw at 100 half-moves)
			HalfmoveClock = 4,

			/// @brief Field 6: Full-move counter (positive integer)
			///
			/// Starts at 1 and increments after Black's move.
			/// Used for move numbering in game notation
			FullmoveCounter = 5
		};

		/// @brief Default constructor initializes an empty position
		///
		/// Creates a position with:
		/// - Empty board (no pieces)
		/// - White to move
		/// - No castling rights
		/// - No en passant square
		/// - Half-move clock = 0
		/// - Full-move number = 1
		///
		/// @note Use set_from_fen() to set up a specific position
		Position() noexcept = default;

		/// @brief Set position from FEN (Forsyth-Edwards Notation) string
		///
		/// Parses a standard FEN string and sets up the complete position including
		/// piece placement and all game state variables.
		///
		/// FEN format consists of 6 space-separated fields:
		/// 1. Piece placement (rank 8 to rank 1, '/' between ranks)
		/// 2. Active color ('w' or 'b')
		/// 3. Castling availability ('KQkq' or '-')
		/// 4. En passant target square (e.g., 'e3' or '-')
		/// 5. Half-move clock (for 50-move rule)
		/// 6. Full-move number
		///
		/// @param fen_string Standard FEN string representing a chess position
		///
		/// @par Example:
		/// @code
		/// Position pos;
		/// // Starting position
		/// pos.set_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		/// // After 1.e4
		/// pos.set_from_fen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
		/// @endcode
		///
		/// @pre Assumes input FEN string is syntactically valid (as guaranteed by UCI-compliant GUIs)
		/// @post Position is fully initialized with all pieces and game state
		///
		/// @warning No validation is performed on the FEN string. Invalid FEN may result
		/// in undefined behavior. Ensure FEN is validated before calling this method.
		///
		/// @note For more information, see https://www.chessprogramming.org/Forsyth-Edwards_Notation
		/// @see clear_position() to reset the position
		auto set_from_fen(std::string_view fen_string) -> void;

		/// @brief Place a piece on the board at the specified square
		///
		/// Updates both the square-centric board array and the piece/color bitboards.
		/// Any existing piece at the target square is overwritten.
		///
		/// @param piece The piece to place (including color and type)
		/// @param square The target square (e.g., Square::E4)
		///
		/// @pre square must be valid (0 <= square < 64)
		/// @post The piece is placed on the board and all bitboards are updated
		///
		/// @note This is a low-level method typically called during position setup.
		/// For move execution, use make_move() instead (when implemented).
		///
		/// @par Example:
		/// @code
		/// Position pos;
		/// pos.create_piece(Piece::WhitePawn, Square::E4);
		/// pos.create_piece(Piece::BlackKing, Square::E8);
		/// @endcode
		/// @relates set_from_fen
		auto create_piece(Piece piece, Square square) -> void;

		/// @brief Reset position to empty state with default game parameters
		///
		/// Clears all pieces from the board and resets all game state to initial values:
		/// - All squares set to empty
		/// - All bitboards cleared
		/// - Side to move: White
		/// - Castling rights: None (unlike starting position which has all rights)
		/// - En passant square: None
		/// - Half-move clock: 0
		/// - Full-move number: 1
		///
		/// @post Position is in a clean empty state, ready for setup
		///
		/// @note This does NOT set up the starting position. Use set_from_fen() with
		/// the starting FEN to initialize a standard game.
		///
		/// @par Example:
		/// @code
		/// Position pos;
		/// pos.set_from_fen("some_fen");
		/// // ... do something ...
		/// pos.clear_position();  // Reset to empty
		/// @endcode
		auto clear_position() -> void;

		/// @brief Set the side to move from FEN character
		/// @param c FEN side character: 'w' for White, 'b' for Black
		///
		/// @pre c must be 'w' or 'b' (as per FEN standard)
		/// @post side_to_move_ is updated to the corresponding color
		///
		/// @note Typically called during FEN parsing. Behavior is undefined
		/// for invalid characters.
		/// @relates set_from_fen
		auto set_side_to_move(char c) -> void;

		/// @brief Get the current side to move
		/// @return The color of the side whose turn it is to move
		///
		/// @note This determines whose move is legal in the current position
		[[nodiscard]] auto get_side_to_move() const noexcept -> Color;

		/// @brief Set castling rights based on FEN character
		///
		/// Processes a single FEN castling character and adds the corresponding
		/// castling right using bitwise OR. This allows building up complete
		/// castling rights by calling this method multiple times with different characters.
		///
		/// @param c FEN castling character:
		///   - 'K': White kingside castling (O-O)
		///   - 'Q': White queenside castling (O-O-O)
		///   - 'k': Black kingside castling
		///   - 'q': Black queenside castling
		///   - '-': No castling rights (typically processed separately)
		///
		/// @post The specified castling right is added to the existing rights
		///
		/// @note Invalid characters are silently ignored. Typically called during
		/// FEN parsing where each character in the castling field is processed.
		///
		/// @par Example:
		/// @code
		/// Position pos;
		/// pos.clear_position();  // Reset to no castling rights
		/// pos.set_castling_rights('K');  // Add White kingside
		/// pos.set_castling_rights('q');  // Add Black queenside
		/// // Result: White can castle kingside, Black can castle queenside
		/// @endcode
		///
		/// @relates set_from_fen
		auto set_castling_rights(char c) -> void;

		/// @brief Get current castling rights
		///
		/// @return Bitmask of available castling rights
		///
		/// @note Return value is a combination of CastlingRights flags.
		/// Use bitwise operations to test for specific rights:
		/// @code
		/// if (pos.get_castling_rights() & CastlingRights::WhiteKingside) {
		///     // White can castle kingside
		/// }
		/// @endcode
		[[nodiscard]] auto get_castling_rights() const noexcept -> CastlingRights;

		/// @brief Set en passant target square from algebraic notation
		///
		/// Parses a file-rank string (e.g., "e3") and sets the en passant square.
		/// The en passant square is the square *behind* the pawn that just moved
		/// two squares forward, not the pawn's current location.
		///
		/// @param file_rank Algebraic notation for the square (e.g., "e3", "d6")
		///                  or "-" to indicate no en passant square
		///
		/// @post enpassant_square_ is set to the parsed square or Square::Size if invalid
		///
		/// @note Format is lowercase file letter (a-h) followed by rank digit (1-8)
		///
		/// @par Example:
		/// @code
		/// Position pos;
		/// // After 1.e4, the en passant square is e3
		/// pos.set_en_passant_square("e3");
		/// // No en passant available
		/// pos.set_en_passant_square("-");
		/// @endcode
		auto set_en_passant_square(const std::string& file_rank) -> void;

		/// @brief Get current en passant target square
		/// @return The square where en passant capture is possible,
		///         or Square::Size if no en passant is available
		///
		/// @note Square::Size represents an invalid square (no en passant)
		[[nodiscard]] auto get_en_passant_square() const noexcept -> Square;

		/// @brief Set half-move clock from FEN character
		///
		/// The half-move clock is used for the 50-move rule (draw when clock reaches 100).
		/// It counts half-moves since the last capture or pawn advance.
		///
		/// @param counter Numeric value of character from FEN string
		///
		/// @post half_move_clock_ is set to the numeric value of the string
		///
		/// @warning Behavior is undefined if counter has any non-numeric characters
		auto set_half_move_clock(const std::string& counter) -> void;

		/// @brief Get current half-move clock value
		///
		/// @return Number of half-moves since last capture or pawn advance
		///
		/// @note Used for enforcing the 50-move rule. When this reaches 100,
		/// the game is drawn (or can be claimed as a draw).
		[[nodiscard]] auto get_half_move_clock() const noexcept -> int;

		/// @brief Set full-move counter from FEN character
		///
		/// The full-move counter starts at 1 and increments after Black's move.
		/// Used for move numbering in chess notation (e.g., "1.e4 e5 2.Nf3").
		///
		/// @param counter Numeric value of character from FEN string
		///
		/// @post full_move_number_ is set to the numeric value of the string
		///
		/// @warning Behavior is undefined if counter has any non-numeric characters
		auto set_full_move_counter(const std::string& counter) -> void;

		/// @brief Get current full-move number
		///
		/// @return The move number in the game (increments after Black's move)
		///
		/// @note This is the number you see in chess notation before White's moves.
		/// Starts at 1 for the game's first move.
		[[nodiscard]] auto get_full_move_counter() const noexcept -> int;

	private:
		/// @brief Square-centric board for "what piece is here?" queries
		std::array<Piece, static_cast<std::size_t>(Square::Size)> board_{Piece::Empty};
		std::array<BitBoard, static_cast<std::size_t>(PieceType::Size)> bitboard_by_piece_{};
		std::array<BitBoard, static_cast<std::size_t>(Color::Size)> bitboard_by_color_{};
		/// @brief For 50-move rule
		///
		/// @note game state variable
		int half_move_clock_{0};
		/// @brief Increments after black's move
		///
		/// @note game state variable
		int full_move_number_{1};
		Color side_to_move_{Color::White};
		CastlingRights castling_rights_{runestone::CastlingRights::None};
		/// @brief Invalid square = no en passant
		///
		/// @note game state variable
		Square enpassant_square_{Square::Size};
		FenStringFields current_fen_string_field_{FenStringFields::PiecePlacement};
	};
}