#include <cstdio>
#include <vector>
#include "NanpureBoard.h"
#include "NanpureSolver.h"

int main(void) {
	int problem[9][9];
	NanpureBoard board;
	std::vector<NanpureBoard> solvedBoards;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (scanf("%d", &problem[i][j]) != 1) {
				puts("invalid input");
				return 1;
			}
			board.putNumber(i, j, problem[i][j]);
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (problem[i][j] != board.getNumber(i, j)) {
				puts("invalid problem");
				return 1;
			}
		}
	}
	solvedBoards = solveNanpure(board);
	printf("%u solution(s) found.\n", (unsigned int)solvedBoards.size());
	for (std::vector<NanpureBoard>::iterator it = solvedBoards.begin();
	it != solvedBoards.end(); it++) {
		puts("-----------------");
		for (int i = 0; i < 9; i++) {
			printf("%d", it->getNumber(i, 0));
			for (int j = 1; j < 9; j++) {
				printf(" %d", it->getNumber(i, j));
			}
			putchar('\n');
		}
	}
	return 0;
}
