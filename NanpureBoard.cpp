#include "NanpureBoard.h"

NanpureBoard::NanpureBoard()  {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			candidatesRow[i][j] = 0x1ff;
			candidatesCol[i][j] = 0x1ff;
			candidatesBox[i][j] = 0x1ff;
			numbersPut[i][j] = 0;
		}
	}
}

int NanpureBoard::getCandidates(int row, int col) const {
	if (row < 0 || 9 <= row || col < 0 || 9 <= col) return 0;
	return candidatesRow[row][col] & candidatesCol[row][col] & candidatesBox[row][col];
}

void NanpureBoard::putNumber(int row, int col, int num) {
	if (row < 0 || 9 <= row || col < 0 || 9 <= col || numbersPut[row][col] != 0) return;
	if (!isNumberValid(row, col, num)) return;
	numbersPut[row][col] = num;
	int mask = ~(1 << (num - 1));
	for (int i = 0; i < 9; i++) {
		candidatesRow[row][i] &= mask;
		candidatesCol[i][col] &= mask;
	}
	int rs = (row / 3) * 3;
	int cs = (col / 3) * 3;
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			candidatesBox[rs + r][cs + c] &= mask;
		}
	}
}

void NanpureBoard::removeNumber(int row, int col) {
	if (row < 0 || 9 <= row || col < 0 || 9 <= col || numbersPut[row][col] == 0) return;
	int mask = 1 << (numbersPut[row][col] - 1);
	numbersPut[row][col] = 0;
	for (int i = 0; i < 9; i++) {
		candidatesRow[row][i] |= mask;
		candidatesCol[i][col] |= mask;
	}
	int rs = (row / 3) * 3;
	int cs = (col / 3) * 3;
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			candidatesBox[rs + r][cs + c] |= mask;
		}
	}
}

int NanpureBoard::getNumber(int row, int col) const {
	if (row < 0 || 9 <= row || col < 0 || 9 <= col) return 0;
	return numbersPut[row][col];
}

bool NanpureBoard::isNumberValid(int row, int col, int num) const {
	if (num < 1 || 9 < num || getNumber(row, col) != 0) return false;
	return ((getCandidates(row, col) >> (num - 1)) & 1) != 0;
}

int NanpureBoard::getValidNumberCount(int row, int col) const {
	if (getNumber(row, col) != 0) return 0;
	int candidates = getCandidates(row, col);
	int count = 0;
	while (candidates > 0) {
		if (candidates & 1) count++;
		candidates >>= 1;
	}
	return count;
}
