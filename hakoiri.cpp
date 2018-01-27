#include <iostream>
#include <array>
#include <queue>
#include <unordered_set>

constexpr int NUM_PIECES = 10;
constexpr int WIDTH = 4;
constexpr int HEIGHT = 5;

struct Size {
    int w, h;
};

constexpr std::array<Size, NUM_PIECES> SIZES = {
    Size{2, 2},
    Size{1, 1},
    Size{1, 1},
    Size{1, 1},
    Size{1, 1},
    Size{1, 2},
    Size{1, 2},
    Size{2, 1},
    Size{2, 1},
    Size{2, 1}
};

struct Position {
    int x, y;
};

struct Board {
    std::array<Position, NUM_PIECES> pieces;
    int numMoves;

    bool isValid(int i) const {
        const Position& a = pieces[i];
        const Size& aSize = SIZES[i];
        for (int j = 0; j < NUM_PIECES; ++j) {
            if (i != j) {
                const Position& b = pieces[j];
                const Size& bSize = SIZES[j];
                if (a.x + aSize.w > b.x &&
                    b.x + bSize.w > a.x &&
                    a.y + aSize.h > b.y &&
                    b.y + bSize.h > a.y) {

                    return false;
                }
            }
        }
        return true;
    }

    bool isSolved() const {
        const Position& m = pieces[0];
        return m.x == 1 && m.y == 3;
    }

    std::uint64_t calculateHash() const {
        std::uint64_t hash = 1ull << positionToShift(pieces[0]);
        for (int i = 1; i <= 4; ++i) {
            hash |= 2ull << positionToShift(pieces[i]);
        }
        for (int i = 5; i <= 6; ++i) {
            hash |= 3ull << positionToShift(pieces[i]);
        }
        for (int i = 7; i <= 9; ++i) {
            hash |= 4ull << positionToShift(pieces[i]);
        }
        return hash;
    }

private:
    static int positionToShift(const Position& pos) {
        return 3 * (pos.x + pos.y * WIDTH);
    }
};

int main() {
    std::queue<Board> queue;
    queue.push({{
        Position{1, 0},
        Position{0, 2},
        Position{0, 4},
        Position{3, 2},
        Position{3, 4},
        Position{0, 0},
        Position{3, 0},
        Position{0, 3},
        Position{1, 2},
        Position{2, 3}
    }, 0});

    std::unordered_set<std::uint64_t> found{queue.front().calculateHash()};

    while (!queue.empty()) {
        const Board& front = queue.front();
        for (int i = 0; i < NUM_PIECES; ++i) {
            const Position& piece = front.pieces[i];
            const Size& size = SIZES[i];
#define MOVE(move, cond) \
            if (cond) { \
                Board next = front; \
                next.pieces[i].move; \
                if (next.isValid(i)) { \
                    ++next.numMoves; \
                    if (next.isSolved()) { \
                        std::cout << next.numMoves << std::endl; \
                        return 0; \
                    } \
                    if (found.emplace(next.calculateHash()).second) { \
                        queue.push(next); \
                    } \
                } \
            }
            MOVE(x++, piece.x + size.w < WIDTH);
            MOVE(x--, piece.x > 0);
            MOVE(y++, piece.y + size.h < HEIGHT);
            MOVE(y--, piece.y > 0);
#undef MOVE
        }
        queue.pop();
    }

    return 0;
}
