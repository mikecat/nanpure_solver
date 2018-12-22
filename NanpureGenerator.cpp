#include <vector>
#include "NanpureGenerator.h"
#include "NanpureSolver.h"

static void putRandomValid(NanpureBoard& nb, int r, int c, int (*rng)(int)) {
	int valids[9];
	int n = nb.getValidNumbers(valids, r, c);
	int idx = rng(n);
	nb.putNumber(r, c, valids[idx]);
}

NanpureBoard generateNanpure(int (*rng)(int)) {
	NanpureBoard answer;
	// set center block randomly
	for (int i = 3; i < 6; i++) {
		for (int j = 3; j < 6; j++) putRandomValid(answer, i, j, rng);
	}
	// set upper center block randomly
	for (int i = 0; i < 3; i++) putRandomValid(answer, i, 3, rng);
	for (int i = 3; i < 6; i++) {
		int c[3], cnum = 0;
		int n = answer.getNumber(i, 5);
		for (int j = 0; j < 3; j++) {
			if (answer.isNumberValid(j, 4, n)) c[cnum++] = j;
		}
		if (cnum > 0) {
			int idx = rng(cnum);
			answer.putNumber(c[idx], 4, n);
		}
	}
	for (int j = 4; j < 6; j++)  {
		for (int i = 0; i < 3; i++) {
			if (answer.getNumber(i, j) == 0) putRandomValid(answer, i, j, rng);
		}
	}
	// set lower center block randomly
	for (int j = 3; j < 6; j++) {
		for (int i = 6; i < 9; i++)putRandomValid(answer, i, j, rng);
	}
	// set left center block randomly
	for (int j = 0; j < 3; j++) putRandomValid(answer, 3, j, rng);
	for (int i = 3; i < 6; i++) {
		int c[3], cnum = 0;
		int n = answer.getNumber(5, i);
		for (int j = 0; j < 3; j++) {
			if (answer.isNumberValid(4, j, n)) c[cnum++] = j;
		}
		if (cnum > 0) {
			int idx = rng(cnum);
			answer.putNumber(4, c[idx], n);
		}
	}
	for (int i = 4; i < 6; i++) {
		for (int j = 0; j < 3; j++)  {
			if (answer.getNumber(i, j) == 0) putRandomValid(answer, i, j, rng);
		}
	}
	// set right center block randomly
	for (int i = 3; i < 6; i++) {
		for (int j = 6; j < 9; j++) putRandomValid(answer, i, j, rng);
	}
	// solve it and choose one
	std::vector<NanpureBoard> answerCandidates = solveNanpure(answer);
	answer = answerCandidates[rng(answerCandidates.size())];
	// greedily remove numbers
	struct coord_t {
		int r, c;
		coord_t(int r_ = 0, int c_ = 0) : r(r_), c(c_) {}
	} removeCandidates[81];
	int removeCandidateNum = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) removeCandidates[removeCandidateNum++] = coord_t(i, j);
	}
	while (removeCandidateNum > 0) {
		int idx = rng(removeCandidateNum);
		coord_t coord = removeCandidates[idx];
		int num = answer.getNumber(coord.r, coord.c);
		answer.removeNumber(coord.r, coord.c);
		std::vector<NanpureBoard> newAnswerCandidates = solveNanpure(answer, 2);
		if (newAnswerCandidates.size() > 1) {
			// solution became non-unique, rollback
			answer.putNumber(coord.r, coord.c, num);
		}
		// became non-unique -> don't choose it again
		// still unique -> cannot choose because it is empty
		removeCandidates[idx] = removeCandidates[--removeCandidateNum];
	}
	return answer;
}
