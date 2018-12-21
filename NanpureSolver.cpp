#include <stack>
#include "NanpureSolver.h"

struct backtrack_t {
	int valids[9], numValids;
	int row, col, idxPut;
	NanpureBoard prevBoard;
	backtrack_t(int *valids_ = 0, int numValids_ = 0,
		int row_ = 0, int col_ = 0, int idxPut_ = 0,
		const NanpureBoard& prevBoard_ = NanpureBoard()) :
			numValids(numValids_), row(row_), col(col_), idxPut(idxPut_), prevBoard(prevBoard_) {
		if (valids_ == 0) {
			numValids = 0;
		} else {
			for (int i = 0; i < numValids; i++) valids[i] = valids_[i];
		}
	}
};

std::vector<NanpureBoard> solveNanpure(const NanpureBoard& board, int returnLimit) {
	std::vector<NanpureBoard> answers;
	std::stack<backtrack_t> backTrack;
	NanpureBoard currentBoard = board;
	for (;;) {
		int minr = 0, minc = 0, minn = 10, minValids[9];
		bool allPut = true, hasError = false;
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				if (currentBoard.getNumber(r, c) == 0) {
					allPut = false;
					int valids[9];
					int n = currentBoard.getValidNumbers(valids, r, c);
					if (n == 0) hasError = true;
					if (n > 0 && n < minn) {
						minr = r;
						minc = c;
						minn = n;
						for (int i = 0; i < n; i++) minValids[i] = valids[i];
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
				if (bt.idxPut + 1 < bt.numValids) {
					currentBoard = bt.prevBoard;
					currentBoard.putNumber(bt.row, bt.col, bt.valids[++bt.idxPut]);
					break;
				}
				backTrack.pop();
			}
		} else {
			if (minn > 1) backTrack.push(backtrack_t(
				minValids, minn, minr, minc, 0, currentBoard));
			currentBoard.putNumber(minr, minc, minValids[0]);
		}
	}
}
