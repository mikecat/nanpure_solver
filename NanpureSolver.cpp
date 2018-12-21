#include <stack>
#include "NanpureSolver.h"

struct backtrack_t {
	int row, col, numPut;
	NanpureBoard prevBoard;
	backtrack_t(int row_ = 0, int col_ = 0, int numPut_ = 0,
		const NanpureBoard& prevBoard_ = NanpureBoard()) :
			row(row_), col(col_), numPut(numPut_), prevBoard(prevBoard_) {}
};

std::vector<NanpureBoard> solveNanpure(const NanpureBoard& board, int returnLimit) {
	std::vector<NanpureBoard> answers;
	std::stack<backtrack_t> backTrack;
	NanpureBoard currentBoard = board;
	for (;;) {
		int minr = 0, minc = 0, minn = 10;
		bool allPut = true, hasError = false;
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				if (currentBoard.getNumber(r, c) == 0) {
					allPut = false;
					int n = currentBoard.getValidNumberCount(r, c);
					if (n == 0) hasError = true;
					if (n > 0 && n < minn) {
						minr = r;
						minc = c;
						minn = n;
					}
				}
			}
		}
		if (minn > 9 || hasError) {
			if (allPut) {
				answers.push_back(currentBoard);
				if (returnLimit > 0 && answers.size() >= (unsigned int)returnLimit) {
					return answers;
				}
			}
			for (;;) {
				if (backTrack.empty()) return answers;
				backtrack_t& bt = backTrack.top();
				currentBoard = bt.prevBoard;
				bool validFound = false;
				for (int i = bt.numPut + 1; i <= 9; i++) {
					if (currentBoard.isNumberValid(bt.row, bt.col, i)) {
						currentBoard.putNumber(bt.row, bt.col, i);
						bt.numPut = i;
						validFound = true;
						break;
					}
				}
				if (validFound) break;
				backTrack.pop();
			}
		} else {
			for (int i = 1; i <= 9; i++) {
				if (currentBoard.isNumberValid(minr, minc, i)) {
					if (minn > 1) backTrack.push(backtrack_t(minr, minc, i, currentBoard));
					currentBoard.putNumber(minr, minc, i);
					break;
				}
			}
		}
	}
}
