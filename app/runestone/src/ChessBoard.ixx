/// @file ChessBoard.ixx
/// @module runestone.chessboard
module;

#include <bit>
#include <cstdint>
#include <utility>

export module runestone.chessboard;

/// @namespace runestone
/// @brief Core board representation and manipulation utilities.
///
/// The `chessboard` namespace defines low-level data structures for representing
/// a chessboard as a 64-bit occupancy map. It includes:
/// - `Square` — enumeration of all 64 board coordinates (A1–H8)
/// - `ChessBoard` — a type-safe, constexpr wrapper around a 64-bit occupancy mask
/// - Predefined file, rank, and diagonal masks for efficient board operations
///
/// These primitives serve as the foundation for higher-level components such as
/// `Position`, move generation, and attack evaluation.
export namespace runestone
{
	/// @class ChessBoard
	/// @brief A 64-bit chessboard representation for square occupancy tracking.
	///
	/// Each bit corresponds to one of the 64 squares (A1–H8), allowing constant-time
	/// bitwise operations for move generation and attack evaluation.
	///
	/// This class is a lightweight, type-safe wrapper around a `uint64_t` that
	/// behaves similarly to `std::bitset<64>` but uses chess terminology and
	/// optimized constexpr operations.
	///
	/// Square mapping convention:
	/// - Bit 0 = A1, Bit 7 = H1, Bit 56 = A8, Bit 63 = H8 (little-endian rank-file)
	///
	/// For more information, see:
	/// - https://www.chessprogramming.org/Bitboards
	/// - https://www.chessprogramming.org/Square_Mapping_Considerations
	class ChessBoard
	{
	public:
		/// @brief squares represents the number of squares on
		/// a chess board (64 squares per chess board).
		using squares = std::uint64_t;

		/// @enum Square
		/// @brief Represents an individual square on a chess board.
		enum class Square : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			// clang-format off
			A1, B1, C1, D1, E1, F1, G1, H1,
			A2, B2, C2, D2, E2, F2, G2, H2,
			A3, B3, C3, D3, E3, F3, G3, H3,
			A4, B4, C4, D4, E4, F4, G4, H4,
			A5, B5, C5, D5, E5, F5, G5, H5,
			A6, B6, C6, D6, E6, F6, G6, H6,
			A7, B7, C7, D7, E7, F7, G7, H7,
			A8, B8, C8, D8, E8, F8, G8, H8,
			Size
			// clang-format on
			/// @endcond
		};

		/// @brief Default-construct an empty chessboard.
		constexpr ChessBoard() noexcept = default;

		/// @brief Construct a chessboard and initialize square occupancy.
		/// @param occupancy Initial chessboard representing the occupied and unoccupied squares.
		constexpr explicit ChessBoard(squares occupancy) noexcept : occupancy_(occupancy)
		{
		}

		/// @brief Return the underlying square occupancy for the chessboard.
		/// @return Internal 64 square value representing all occupied and unoccupied squares.
		[[nodiscard]] constexpr auto occupancies() const noexcept -> squares
		{
			return occupancy_;
		}

		/// @brief Check whether this chessboard is empty (no square is occupied).
		/// @return True if all squares are unoccupied, false otherwise.
		[[nodiscard]] constexpr auto empty() const noexcept -> bool
		{
			return occupancy_ == 0ULL;
		}

		/// @brief Count the number of squares that are occupied.
		/// @return Number of occupied squares in the chessboard.
		[[nodiscard]] constexpr auto size() const noexcept -> int
		{
			return std::popcount(occupancy_);
		}

		/// @brief Occupy the square corresponding to the given square.
		/// @param square Square index (0–63).
		constexpr auto occupy_square(Square square) noexcept -> void
		{
			occupancy_ |= square_mask(square);
		}

		/// @brief Vacate the square corresponding to the given square.
		/// @param square Square index (0–63).
		constexpr auto vacate_square(Square square) noexcept -> void
		{
			occupancy_ &= ~(square_mask(square));
		}

		/// @brief Flip (invert) the occupancy of a square at a given square.
		/// @param square Square index (0–63).
		constexpr auto flip_square_occupancy(Square square) noexcept -> void
		{
			occupancy_ ^= square_mask(square);
		}

		/// @brief Check whether the square is occupied corresponding to the given square.
		/// @param square Square index (0–63).
		/// @return True if the square is occupied, false otherwise.
		[[nodiscard]] constexpr auto occupied(Square square) const noexcept -> bool
		{
			return ((occupancy_ >> static_cast<squares>(square)) & 1ULL) != 0ULL;
		}

		/// @brief Find the first occupied square. Search of chessboard moves from FileA and Rank1
		/// to FileH and Rank8 (i.e. A1 -> H1 -> A2 -> H2 -> .... -> A7 -> H7 -> A8 -> H8).
		/// @return Square index [0–63] of the first occupied square.
		/// @pre ChessBoard must not be empty (undefined if `occupancy_ == 0`).
		[[nodiscard]] constexpr auto find_first_occupancy() const noexcept -> int
		{
			if (occupancy_ == 0ULL)
			{
				std::unreachable();
			}
			return std::countr_zero(occupancy_);
		}

		/// @brief Find the last occupied square. Search of chessboard moves from FileH and Rank8
		/// to FileA and Rank1 (i.e. H8 -> A8 -> H7 -> A7 -> ... -> H2 -> A2 -> H1 -> A1).
		/// @return Square index [0–63] of the last occupied square.
		/// @pre ChessBoard must not be empty (undefined if `value_ == 0`).
		[[nodiscard]] constexpr auto find_last_occupancy() const noexcept -> int
		{
			if (occupancy_ == 0ULL)
			{
				std::unreachable();
			}
			return static_cast<int>(Square::Size) - 1 - std::countl_zero(occupancy_);
		}

		/// @brief Pop (remove and return) the first occupied square.
		/// @return Square index [0–63] that was cleared.
		/// @post Removes the first occupied square from the chessboard.
		constexpr auto pop_first_occupancy() noexcept -> int
		{
			const auto square = find_first_occupancy();
			occupancy_ &= (occupancy_ - 1);
			return square;
		}

		/// @brief Return the union of two chessboards.
		/// @param rhs Right-hand ChessBoard operand to merge with this one.
		/// @return A new ChessBoard representing the union of `*this`'s and `rhs`'s square occupancies.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard whitePieces(0x000000000000FFFFULL);
		/// ChessBoard blackPieces(0xFFFF000000000000ULL);
		/// ChessBoard occupied = whitePieces.union_occupancies(blackPieces);
		/// // occupied now contains all squares with any occupancy.
		/// @endcode
		[[nodiscard]] constexpr auto union_occupancies(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(occupancy_ | rhs.occupancy_);
		}

		/// @brief Return the intersection of two chessboards.
		/// @param rhs Right-hand ChessBoard operand to intersect with this one.
		/// @return A new ChessBoard representing the intersection of `*this`'s and `rhs`'s square occupancies.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard whiteRooks(0x0000000000000081ULL);  // A1, H1
		/// ChessBoard occupiedFiles(0x0101010101010101ULL); // A-file
		/// ChessBoard result = whiteRooks.intersect_occupancies(occupiedFiles);
		/// // result will only have one occupancy (square index A1)
		/// @endcode
		[[nodiscard]] constexpr auto intersect_occupancies(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(occupancy_ & rhs.occupancy_);
		}

		/// @brief Return the symmetric difference of two chessboards.
		/// @param rhs Right-hand ChessBoard operand to combine with this one.
		/// @return A new ChessBoard representing the symmetric difference of `*this`'s and `rhs`'s square occupancies.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard beforeMove(0x0000000000000010ULL); // Piece on E1
		/// ChessBoard afterMove (0x0000000000100000ULL); // Piece on E4
		/// ChessBoard diff = beforeMove.sdiff_occupancies(afterMove);
		/// // diff has bits set for both E1 and E4 — the squares that changed
		/// @endcode
		[[nodiscard]] constexpr auto sdiff_occupancies(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(occupancy_ ^ rhs.occupancy_);
		}

		/// @brief Return a ChessBoard with all square occupancies flipped.
		/// @return A new ChessBoard representing the complement of `*this`'s square occupancies.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard occupied(0x00000000000000FFULL);  // Rank 1 occupied
		/// ChessBoard empty = occupied.flip_occupancies();          // All squares except rank 1 are set
		/// @endcode
		[[nodiscard]] constexpr auto flip_occupancies() const noexcept -> ChessBoard
		{
			return ChessBoard(~occupancy_);
		}

		/// @brief Perform in-place union.
		///
		/// Updates this ChessBoard's square occupancies by performing a union with another's.
		/// The result is stored in `*this`.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one's.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto union_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			occupancy_ |= rhs.occupancy_;
			return *this;
		}

		/// @brief Perform in-place intersection.
		///
		/// Updates this ChessBoard's square occupancies by performing an intersection with another's.
		/// The result is stored in `*this`. Only squares occupied in both operands remain set.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one's.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto intersection_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			occupancy_ &= rhs.occupancy_;
			return *this;
		}

		/// @brief Perform in-place symmetric difference.
		///
		/// Updates this ChessBoard's square occupancies by performing a symmetric difference with another's.
		/// Each square is occupied if it is occupied in exactly one operand.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto sdiff_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			occupancy_ ^= rhs.occupancy_;
			return *this;
		}

		/// @brief Check if two ChessBoards are equal.
		///
		/// Compares the square occupancies of this ChessBoard with another.
		/// Returns true if both represent the exact same set of occupied squares.
		///
		/// @param rhs The ChessBoard to compare against.
		/// @return True if both ChessBoards have identical occupied squares, false otherwise.
		[[nodiscard]] constexpr auto equals(const ChessBoard& rhs) const noexcept -> bool
		{
			return occupancy_ == rhs.occupancy_;
		}

		/// @brief Check if two ChessBoards differ in square occupancies.
		///
		/// Compares the square occupancies of this ChessBoard with another.
		/// Returns true if their square occupancies differ in at least one square.
		///
		/// @param rhs The ChessBoard to compare against.
		/// @return True if the ChessBoards differ, false if they are identical.
		[[nodiscard]] constexpr auto not_equals(const ChessBoard& rhs) const noexcept -> bool
		{
			return occupancy_ != rhs.occupancy_;
		}

		/// @defgroup constants Chess Board Constants
		/// @brief Common chessboard occupancy masks.
		///
		/// These constants define frequently used occupancy patterns for files, ranks,
		/// board edges, diagonals, and color squares. They are used during move
		/// generation, attack table construction, and board evaluation.
		///
		/// For more information, see:
		/// - https://www.chessprogramming.org/Bitboards
		/// - https://www.chessprogramming.org/Board_Representation
		/// @{

		/// @brief Represents the squares on a chessboard for the A file.
		static constexpr squares AFile{0x0101010101010101ULL};
		/// @brief Represents the squares on a chessboard for the B file.
		static constexpr squares BFile{AFile << 1U};
		/// @brief Represents the squares on a chessboard for the C file.
		static constexpr squares CFile{AFile << 2U};
		/// @brief Represents the squares on a chessboard for the D file.
		static constexpr squares DFile{AFile << 3U};
		/// @brief Represents the squares on a chessboard for the E file.
		static constexpr squares EFile{AFile << 4U};
		/// @brief Represents the squares on a chessboard for the F file.
		static constexpr squares FFile{AFile << 5U};
		/// @brief Represents the squares on a chessboard for the G file.
		static constexpr squares GFile{AFile << 6U};
		/// @brief Represents the squares on a chessboard for the H file.
		static constexpr squares HFile{AFile << 7U};

		/// @brief Represents the squares on a chessboard for the 1st Rank.
		static constexpr squares Rank1{0xFFULL};
		/// @brief Represents the squares on a chessboard for the 2nd Rank.
		static constexpr squares Rank2{Rank1 << (8 * 1U)};
		/// @brief Represents the squares on a chessboard for the 3rd Rank.
		static constexpr squares Rank3{Rank1 << (8 * 2U)};
		/// @brief Represents the squares on a chessboard for the 4th Rank.
		static constexpr squares Rank4{Rank1 << (8 * 3U)};
		/// @brief Represents the squares on a chessboard for the 5th Rank.
		static constexpr squares Rank5{Rank1 << (8 * 4U)};
		/// @brief Represents the squares on a chessboard for the 6th Rank.
		static constexpr squares Rank6{Rank1 << (8 * 5U)};
		/// @brief Represents the squares on a chessboard for the 7th Rank.
		static constexpr squares Rank7{Rank1 << (8 * 6U)};
		/// @brief Represents the squares on a chessboard for the 8th Rank.
		static constexpr squares Rank8{Rank1 << (8 * 7U)};

		/// @brief Represents a chessboard's squares for all files except File A.
		static constexpr squares NotAFile{~AFile};
		/// @brief Represents a chessboard's squares for all files except File H.
		static constexpr squares NotHFile{~HFile};
		/// @brief Represents a chessboard's squares for all ranks except Rank 1.
		static constexpr squares NotRank1{~Rank1};
		/// @brief Represents a chessboard's squares for all ranks except Rank 8.
		static constexpr squares NotRank8{~Rank8};

		/// @brief Represents an empty chessboard (zero squares are occupied).
		static constexpr squares Empty{0x0000000000000000ULL};
		/// @brief Represents a full chessboard (one piece occupies every square).
		static constexpr squares Full{0xFFFFFFFFFFFFFFFFULL};
		/// @brief Represents a chessboard where every light square has a piece occupying it.
		static constexpr squares LightSquares{0x55AA55AA55AA55AAULL};
		/// @brief Represents a chessboard where every dark square has a piece occupying it.
		static constexpr squares DarkSquares{0xAA55AA55AA55AA55ULL};
		/// @brief Represents a chessboard where every square along the diagonal has a piece occupying it.
		static constexpr squares DiagonalA1H8{0x8040201008040201ULL};
		/// @brief Represents a chessboard where every square along the diagonal has a piece occupying it.
		static constexpr squares AntiDiagonalH1A8{0x0102040810204080ULL};
		/// @}

	private:
		/// @brief Return a chessboard mask with a single bit set at the given square.
		/// @param square Square index (0–63).
		/// @return A 64-bit chessboard with only the bit at `square` set.
		[[nodiscard]] static constexpr auto square_mask(Square square) noexcept -> squares
		{
			return 1ULL << static_cast<squares>(square);
		}

		squares occupancy_{0ULL};
	};
}
