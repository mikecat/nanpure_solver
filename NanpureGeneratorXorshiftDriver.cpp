#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cinttypes>
#include <string>
#include <vector>
#include "NanpureGenerator.h"
#include "NanpureBoard.h"

uint32_t x = 123456789;
uint32_t y = 362436069;
uint32_t z = 521288629;
uint32_t w = 88675123;

uint32_t xorshift(void) {
	uint32_t t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return w;
}

void xorshift_seed(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
	if ((a|b|c|d) != 0) {
		x = a;
		y = b;
		z = c;
		w = d;
		// reduce direct influence of seed values
		for (int i = 0; i < 100; i++) xorshift();
	}
}

int rng(int range) {
	if (range <= 0) {
		puts("rng error: range <= 0");
		exit(1);
	}
	uint32_t range_u = range;
	uint32_t rest = UINT32_MAX % range_u;
	uint32_t result;
	do {
		result = xorshift();
	} while (result < rest);
	return result % range_u;
}

enum output_format_t {
	NORMAL,
	ONELINE,
	PDF
};

struct pdf_config_t {
	double pageWidth;
	double pageHeight;
	double boldLineWidth;
	double normalLineWidth;
	double numberLineWidth;
	double blockSize;
	unsigned int numProblemsPerPageX;
	unsigned int numProblemsPerPageY;

	pdf_config_t() :
		pageWidth(210.0), pageHeight(297.0),
		boldLineWidth(2.0), normalLineWidth(0.7), numberLineWidth(1.0),
		blockSize(15.0),
		numProblemsPerPageX(1), numProblemsPerPageY(1) {}
};

std::string double_to_string(double v) {
	char str[512];
	snprintf(str, sizeof(str), "%.3f", v);
	return str;
}

std::string uint_to_string(unsigned int v) {
	char str[512];
	snprintf(str, sizeof(str), "%u", v);
	return str;
}

std::string create_pdf(const std::vector<NanpureBoard>& nbs, const char* seedString,
const pdf_config_t& config) {
	const static signed char numData1[] =
		{55, 20, -'m', 45, 85, -'l', 0};
	const static signed char numData2[] =
		{30, 35, -'m', 35, 15, 80, 23, 60, 50, -'c', 30, 85, -'l', 70, 85, -'l', 0};
	const static signed char numData3[] =
		{30, 28, -'m', 35, 15, 70, 15, 70, 35, -'c', 70, 50, 50, 50, 40, 50, -'c', 50, 50, 70, 50, 70, 65, -'c', 70, 85, 35, 85, 30, 72, -'c', 0};
	const static signed char numData4[] =
		{55, 15, -'m', 45, 85, -'l', 30, 15, -'m', 25, 50, -'l', 75, 50, -'l', 0};
	const static signed char numData5[] =
		{70, 20, -'m', 30, 20, -'l', 25, 55, -'l', 30, 45, 48, 45, 50, 45, -'c', 65, 45, 70, 60, 70, 65, -'c', 70, 90, 40, 90, 30, 80, -'c', 0};
	const static signed char numData6[] =
		{45, 15, -'m', 30, 30, 28, 40, 25, 50, -'c', 21, 65, 35, 85, 45, 85, -'c', 60, 85, 70, 75, 70, 65, -'c', 70, 55, 60, 45, 50, 45, -'c', 35, 45, 30, 48, 25, 50, -'c', 0};
	const static signed char numData7[] =
		{30, 40, -'m', 32, 20, -'l', 70, 20, -'l', 60, 85, -'l', 0};
	const static signed char numData8[] =
		{50, 15, -'m', 80, 15, 55, 40, 50, 45, -'c', 45, 50, 15, 85, 50, 85, -'c', 85, 85, 55, 50, 50, 45, -'c', 45, 45, 20, 15, 50, 15, -'c', 0};
	const static signed char numData9[] =
		{70, 45, -'m', 80, 25, 60, 15, 50, 15, -'c', 40, 15, 30, 25, 30, 35, -'c', 30, 45, 40, 55, 50, 55, -'c', 53, 55, 65, 55, 70, 45, -'c', 50, 85, -'l', 0};
	const static signed char* numData[] = {
		NULL, numData1, numData2, numData3, numData4, numData5, numData6, numData7, numData8, numData9
	};

	const double multi = 72 / 25.4; // length[mm] * multi = coordinate value
	// [mm]
	const double pageWidth = config.pageWidth;
	const double pageHeight = config.pageHeight;
	const double boldLineWidth = config.boldLineWidth;
	const double normalLineWidth = config.normalLineWidth;
	const double numberLineWidth = config.numberLineWidth;
	const double blockSize = config.blockSize;
	const unsigned int numProblemsPerPageX = config.numProblemsPerPageX;
	const unsigned int numProblemsPerPageY = config.numProblemsPerPageY;

	double pageWidthPerProblem = pageWidth / numProblemsPerPageX;
	double pageHeightPerProblem = pageHeight / numProblemsPerPageY;
	double problemSize = blockSize * 9;

	std::string pdfData = "%PDF-1.4\r\n";
	std::vector<std::string> pdfContents;

	std::vector<std::string> drawScripts;
	std::string drawScript;
	unsigned int xIndex = 0, yIndex = 0;
	for (std::vector<NanpureBoard>::const_iterator it = nbs.begin(); it != nbs.end(); it++) {
		if (xIndex == 0 && yIndex == 0) {
			drawScript = "q\r\n1 0 0 1 0 0 cm\r\n0 G\r\n";
		}

		double startX = pageWidthPerProblem * xIndex + (pageWidthPerProblem - problemSize) / 2.0;
		double startY = pageHeightPerProblem * (numProblemsPerPageY - 1 - yIndex) +
			(pageHeightPerProblem - problemSize) / 2.0;
		std::string x1Str = double_to_string((startX - boldLineWidth / 2.0) * multi);
		std::string x2Str = double_to_string((startX + problemSize + boldLineWidth / 2.0) * multi);
		std::string y1Str = double_to_string((startY) * multi);
		std::string y2Str = double_to_string((startY + problemSize) * multi);
		drawScript += double_to_string(normalLineWidth * multi) + " w\r\n";
		for (int i = 0; i <= 9; i++) {
			if (i % 3 == 0) continue;
			std::string xStr = double_to_string((startX + blockSize * i) * multi);
			std::string yStr = double_to_string((startY + blockSize * i) * multi);
			drawScript += x1Str + " " + yStr + " m " + x2Str + " " + yStr + " l S\r\n";
			drawScript += xStr + " " + y1Str + " m " + xStr + " " + y2Str + " l S\r\n";
		}
		drawScript += double_to_string(boldLineWidth * multi) + " w\r\n";
		for (int i = 0; i <= 9; i += 3) {
			std::string xStr = double_to_string((startX + blockSize * i) * multi);
			std::string yStr = double_to_string((startY + blockSize * i) * multi);
			drawScript += x1Str + " " + yStr + " m " + x2Str + " " + yStr + " l S\r\n";
			drawScript += xStr + " " + y1Str + " m " + xStr + " " + y2Str + " l S\r\n";
		}

		drawScript += double_to_string(numberLineWidth * multi) + " w\r\n";
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				const signed char* numDraw = numData[it->getNumber(x, 8 - y)];
				if (numDraw != NULL) {
					double offsetX = startX + blockSize * x;
					double offsetY = startY + blockSize * (y + 1);
					bool isX = true;
					while (*numDraw != 0) {
						if (*numDraw > 0) {
							double offset = blockSize * (*numDraw) / 100.0;
							drawScript += double_to_string((isX ? offsetX + offset : offsetY - offset) * multi);
							isX = !isX;
						} else {
							drawScript += (char)(-*numDraw);
						}
						drawScript += ' ';
						numDraw++;
					}
					drawScript += "S\r\n";
				}
			}
		}

		if (++xIndex >= numProblemsPerPageX) {
			xIndex = 0;
			if (++yIndex >= numProblemsPerPageY) {
				yIndex = 0;
			}
		}
		if ((xIndex == 0 && yIndex == 0) || it + 1 == nbs.end()) {
			drawScript += "Q";
			drawScripts.push_back(drawScript);
		}
	}

	pdfContents.push_back("1 0 obj\r\n<</Title(Nanpure)/Subject(" +
		std::string(seedString) + ")>>\r\nendobj\r\n");
	pdfContents.push_back("2 0 obj\r\n<</ProcSet[/PDF/Text/ImageC/ImageB/ImageI]>>\r\nendobj\r\n");
	pdfContents.push_back("3 0 obj\r\n<</Pages 4 0 R/Type/Catalog>>\r\nendobj\r\n");
	std::string pages = "4 0 obj\r\n<</Type/Pages/Count " +
		uint_to_string(drawScripts.size()) + "/Kids[";
	for (size_t i = 0; i < drawScripts.size(); i++) {
		if (i > 0) pages += ' ';
		pages += uint_to_string(i * 2 + 5) + " 0 R";
	}
	pages += "]/MediaBox[0 0 " + double_to_string(pageWidth * multi) + " " +
			double_to_string(pageHeight * multi) + "]>>\r\nendobj\r\n";
	pdfContents.push_back(pages);

	for (size_t i = 0; i < drawScripts.size(); i++) {
		pdfContents.push_back(uint_to_string(i * 2 + 5) +
			" 0 obj\r\n<</Resources 2 0 R/Type/Page/Parent 4 0 R/Contents[" +
			uint_to_string(i * 2 + 5 + 1) + " 0 R]>>\r\nendobj\r\n");
		pdfContents.push_back(uint_to_string(i * 2 + 5 + 1) +
			" 0 obj\r\n<</Length " + uint_to_string(drawScripts[i].size()) +
			">>\r\nstream\r\n" + drawScripts[i] + "\r\nendstream\r\nendobj\r\n");
	}

	std::vector<size_t> objStarts;
	for (std::vector<std::string>::const_iterator it = pdfContents.begin();
	it != pdfContents.end(); it++) {
		objStarts.push_back(pdfData.size());
		pdfData += *it;
	}
	size_t xrefStart = pdfData.size();
	pdfData += "xref\r\n0 " + uint_to_string(objStarts.size() + 1) + "\r\n0000000000 65535 f\r\n";
	for (size_t i = 0; i < objStarts.size(); i++) {
		char objStartStr[32];
		snprintf(objStartStr, sizeof(objStartStr), "%010u 00000 n\r\n", (unsigned int)objStarts[i]);
		pdfData += objStartStr;
	}
	pdfData += "trailer\r\n<</Root 3 0 R/Info 1 0 R/Size " +
		uint_to_string(objStarts.size()) + ">>\r\nstartxref\r\n";
	pdfData += uint_to_string(xrefStart) + "\r\n%%EOF\r\n";

	return pdfData;
}

int main(int argc, char* argv[]) {
	const static struct paper_t {
		char name[24];
		unsigned int width, height;
	} papers[] = {
		{"a0", 841, 1189}, {"a0_h", 1189, 841},
		{"a1", 594, 841}, {"a1_h", 841, 594},
		{"a2", 420, 594}, {"a2_h", 594, 420},
		{"a3", 297, 420}, {"a3_h", 420, 297},
		{"a4", 210, 297}, {"a4_h", 297, 210},
		{"a5", 148, 210}, {"a5_h", 210, 148},
		{"a6", 105, 148}, {"a6_h", 148, 105},
		{"a7", 74, 105}, {"a7_h", 105, 74},
		{"a8", 52, 74}, {"a8_h", 74, 52},
		{"a9", 37, 52}, {"a9_h", 52, 37},
		{"a10", 26, 37}, {"a10_h", 37, 26},

		{"b0", 1030, 1456}, {"b0_h", 1456, 1030},
		{"b1", 728, 1030}, {"b1_h", 1030, 728},
		{"b2", 515, 728}, {"b2_h", 728, 515},
		{"b3", 364, 515}, {"b3_h", 515, 364},
		{"b4", 257, 364}, {"b4_h", 364, 257},
		{"b5", 182, 257}, {"b5_h", 257, 182},
		{"b6", 128, 182}, {"b6_h", 182, 128},
		{"b7", 91, 128}, {"b7_h", 128, 91},
		{"b8", 64, 91}, {"b8_h", 91, 64},
		{"b9", 45, 64}, {"b9_h", 64, 45},
		{"b10", 32, 45}, {"b10_h", 45, 32},

		{"postcard", 100, 148}, {"postcard_h", 148, 100},
		{"dpostcard", 148, 200}, {"dpostcard_h", 200, 148},
		{"dsc", 89, 119}, {"dsc_h", 119, 89},
		{"l", 89, 127}, {"l_h", 127, 89},
		{"2l", 127, 178}, {"2l_h", 178, 127},

		{"dummy", 0, 0}
	};

	uint32_t a = 0, b = 0, c = 0, d  = 0;
	bool seed_set = false;
	output_format_t output_format = NORMAL;
	unsigned int output_num = 1;
	char* output_file = NULL;
	pdf_config_t config;
	bool auto_line_width = false;
	bool auto_page_size = false;
	double marginX = -1, marginY = -1;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--seed") == 0 || strcmp(argv[i], "-s") == 0) {
			if (++i < argc) {
				if (sscanf(argv[i], "%" SCNx32 "-%" SCNx32 "-%" SCNx32 "-%" SCNx32, &a, &b, &c, &d) != 4 || (a|b|c|d) == 0) {
					puts("invalid seed");
					return 1;
				}
				seed_set = true;
			} else {
				puts("no seed value");
				return 1;
			}
		} else if (strcmp(argv[i], "--outformat") == 0 || strcmp(argv[i], "-f") == 0) {
			if (++i < argc) {
				if (strcmp(argv[i], "normal") == 0) output_format = NORMAL;
				else if (strcmp(argv[i], "oneline") == 0) output_format = ONELINE;
				else if (strcmp(argv[i], "pdf") == 0) output_format = PDF;
				else {
					puts("unknown output format");
					return 1;
				}
			} else {
				puts("no output format");
				return 1;
			}
		} else if (strcmp(argv[i], "--outfile") == 0 || strcmp(argv[i], "-o") == 0) {
			if (++i < argc) {
				output_file = argv[i];
			} else {
				puts("no output file name");
				return 1;
			}
		} else if (strcmp(argv[i], "--outnum") == 0 || strcmp(argv[i], "-n") == 0) {
			if (++i < argc) {
				if (sscanf(argv[i], "%u", &output_num) != 1 || output_num <= 0) {
					puts("invalid output number");
					return 1;
				}
			} else {
				puts("no output file name");
				return 1;
			}
#define READ_POSITIVE(param, format, desc) \
		} else if (strcmp(argv[i], "--" #param) == 0) { \
			if (++i < argc) { \
				if (sscanf(argv[i], format, &config.param) != 1 || config.param <= 0) { \
					puts("invalid " desc); \
					return 1; \
				} \
			} else { \
				puts("no " desc); \
				return 1; \
			}
		READ_POSITIVE(pageWidth, "%lf", "PDF page width")
		READ_POSITIVE(pageHeight, "%lf", "PDF page height")
		READ_POSITIVE(boldLineWidth, "%lf", "bold line width for PDF")
		READ_POSITIVE(normalLineWidth, "%lf", "normal line width for PDF")
		READ_POSITIVE(numberLineWidth, "%lf", "number line width for PDF")
		READ_POSITIVE(blockSize, "%lf", "one block size for PDF")
		READ_POSITIVE(numProblemsPerPageX, "%u", "number of columns of problems per one page of PDF")
		READ_POSITIVE(numProblemsPerPageY, "%u", "number of rows of problems per one page of PDF")
#undef READ_POSITIVE
		} else if (strcmp(argv[i], "--autoLineWidth") == 0) {
			auto_line_width = true;
#define READ_NONNEGATIVE(variable, format, desc) \
		} else if (strcmp(argv[i], "--" #variable) == 0) { \
			if (++i < argc) { \
				if (sscanf(argv[i], format, &variable) != 1 || variable < 0) { \
					puts("invalid " desc); \
					return 1; \
				} \
			} else { \
				puts("no " desc); \
				return 1; \
			}
		READ_NONNEGATIVE(marginX, "%lf", "horizontal margin for PDF")
		READ_NONNEGATIVE(marginY, "%lf", "vertical margin for PDF")
#undef READ_NONNEGATIVE
		} else if (strcmp(argv[i], "--pageSize") == 0) {
			if (++i < argc) {
				if (strcmp(argv[i], "auto") == 0) {
					auto_page_size = true;
				} else {
					bool found = false;
					for (int j = 0; papers[j].width > 0; j++) {
						if (strcmp(argv[i], papers[j].name) == 0) {
							config.pageWidth = papers[j].width;
							config.pageHeight = papers[j].height;
							found = true;
							break;
						}
					}
					if (!found) {
						printf("unknown page size name %s\n", argv[i]);
						return 1;
					}
					auto_page_size = false;
				}
			} else {
				puts("no page size name");
				return 1;
			}
		} else {
			printf("unknown parameter %s\n", argv[i]);
			return 1;
		}
	}
	if (auto_page_size) {
		if (auto_line_width) config.boldLineWidth = config.blockSize * 2.0 / 15.0;
		if (marginX < 0) marginX = config.boldLineWidth / 2.0;
		if (marginY < 0) marginY = config.boldLineWidth / 2.0;
	}
	if (marginX >= 0 || marginY >= 0) {
		if (marginX < 0) marginX = 0;
		if (marginY < 0) marginY = 0;
		double allMarginX = marginX * 2 * config.numProblemsPerPageX;
		double allMarginY = marginY * 2 * config.numProblemsPerPageY;
		if (auto_page_size) {
			config.pageWidth = config.blockSize * 9 * config.numProblemsPerPageX + allMarginX;
			config.pageHeight = config.blockSize * 9 * config.numProblemsPerPageY + allMarginY;
		} else {
			if (allMarginX > config.pageWidth || allMarginY > config.pageHeight) {
				puts("margin too large");
				return 1;
			}
			double sizeFromX = (config.pageWidth - allMarginX) / config.numProblemsPerPageX / 9.0;
			double sizeFromY = (config.pageHeight - allMarginY) / config.numProblemsPerPageY / 9.0;
			config.blockSize = (sizeFromX <= sizeFromY ? sizeFromX : sizeFromY);
		}
	}
	if (auto_line_width) {
		config.boldLineWidth = config.blockSize * 2.0 / 15.0;
		config.normalLineWidth = config.blockSize * 0.7 / 15.0;
		config.numberLineWidth = config.blockSize * 1.0 / 15.0;
	}
	if (!seed_set) {
		uint32_t t = (uint32_t)time(NULL);
		xorshift_seed(t, t + 1, t + 2, t + 3);
		do {
			a = xorshift();
			b = xorshift();
			c = xorshift();
			d = xorshift();
		} while ((a|b|c|d) == 0);
	}
	xorshift_seed(a, b, c, d);
	char seedString[64];
	snprintf(seedString, sizeof(seedString),
		"seed = %08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "-%08" PRIX32, a, b, c, d);
	std::vector<NanpureBoard> nbs;
	for (unsigned int i = 0; i < output_num; i++) {
		nbs.push_back(generateNanpure(rng));
	}
	FILE* fp;
	if (output_file != NULL) {
		fp = fopen(output_file, output_format == PDF ? "wb" : "w");
		if (fp == NULL) {
			puts("output file open failed");
			return 1;
		}
	} else {
		fp = stdout;
	}
	switch (output_format) {
		case NORMAL:
			fprintf(fp, "%s\n", seedString);
			for (std::vector<NanpureBoard>::const_iterator it = nbs.begin();
			it != nbs.end(); it++) {
				fputc('\n', fp);
				for (int i = 0; i < 9; i++) {
					fprintf(fp, "%d", it->getNumber(i, 0));
					for (int j = 1; j < 9; j++) {
						fprintf(fp, " %d", it->getNumber(i, j));
					}
					fputc('\n', fp);
				}
			}
			break;
		case ONELINE:
			fprintf(fp, "%s\n", seedString);
			for (std::vector<NanpureBoard>::const_iterator it = nbs.begin();
			it != nbs.end(); it++) {
				fputc('\n', fp);
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						fprintf(fp, "%d", it->getNumber(i, j));
					}
				}
				fputc('\n', fp);
			}
			break;
		case PDF:
			{
				std::string pdfData = create_pdf(nbs, seedString, config);
				fwrite(pdfData.data(), 1, pdfData.size(), fp);
			}
	}
	fclose(fp);
	return 0;
}
