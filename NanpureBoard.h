#ifndef NANPURE_SOLVER_H_GUARD_97ED6892_F278_42ED_8373_182D791BFFBE
#define NANPURE_SOLVER_H_GUARD_97ED6892_F278_42ED_8373_182D791BFFBE

class NanpureBoard {
private:
	int candidatesRow[9][9];
	int candidatesCol[9][9];
	int candidatesBox[9][9];
	int numbersPut[9][9];
	int getCandidates(int row, int col) const;
public:
	NanpureBoard();
	void putNumber(int row, int col, int num);
	void removeNumber(int row, int col);
	int getNumber(int row, int col) const;
	bool isNumberValid(int row, int col, int num) const;
	int getValidNumberCount(int row, int col) const;
	int getValidNumbers(int* nums, int row, int col) const;
};

#endif
