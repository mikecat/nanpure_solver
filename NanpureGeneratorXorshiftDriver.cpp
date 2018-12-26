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

std::string double_to_string(double v) {
	char str[512];
	snprintf(str, sizeof(str), "%.3f", v);
	return str;
}

std::string create_pdf(const NanpureBoard& nb, const char* seedString) {
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
	double pageWidth = 210.0;
	double pageHeight = 297.0;
	double boldLineWidth = 2.0;
	double normalLineWidth = 0.7;
	double numberLineWidth = 1.0;
	double blockSize = 15.0;

	std::string pdfData = "%PDF-1.4\r\n";
	std::vector<std::string> pdfContents;
	std::string drawScript = "q\r\n1 0 0 1 0 0 cm\r\n0 G\r\n";

	double startX = (pageWidth - blockSize * 9) / 2.0;
	double startY = (pageHeight - blockSize * 9) / 2.0;
	std::string x1Str = double_to_string((startX - boldLineWidth / 2.0) * multi);
	std::string x2Str = double_to_string((startX + blockSize * 9 + boldLineWidth / 2.0) * multi);
	std::string y1Str = double_to_string((startY) * multi);
	std::string y2Str = double_to_string((startY + blockSize * 9) * multi);
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
			const signed char* numDraw = numData[nb.getNumber(x, 8 - y)];
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

	drawScript += "Q\r\n";
	char drawScriptLength[16];
	snprintf(drawScriptLength, sizeof(drawScriptLength), "%u", (unsigned int)drawScript.size());
	pdfContents.push_back("1 0 obj\r\n<</Length " + std::string(drawScriptLength) +
		">>\r\nstream\r\n" + drawScript + "endstream\r\nendobj\r\n");
	pdfContents.push_back("2 0 obj\r\n<</ProcSet[/PDF/Text/ImageC/ImageB/ImageI]>>\r\nendobj\r\n");
	pdfContents.push_back("3 0 obj\r\n<</Resources 2 0 R/Type/Page/Parent 4 0 R/Contents[1 0 R]>>\r\nendobj\r\n");
	pdfContents.push_back("4 0 obj\r\n<</Type/Pages/Count 1/Kids[3 0 R]/MediaBox[0 0 " +
		double_to_string(pageWidth * multi) + " " + double_to_string(pageHeight * multi) + "]>>\r\nendobj\r\n");
	pdfContents.push_back("5 0 obj\r\n<</Pages 4 0 R/Type/Catalog>>\r\nendobj\r\n");
	pdfContents.push_back("6 0 obj\r\n<</Title(Nanpure)/Subject(" +
		std::string(seedString) + ")>>\r\nendobj\r\n");

	std::vector<size_t> objStarts;
	for (std::vector<std::string>::const_iterator it = pdfContents.begin();
	it != pdfContents.end(); it++) {
		objStarts.push_back(pdfData.size());
		pdfData += *it;
	}
	size_t xrefStart = pdfData.size();
	pdfData += "xref\r\n0 7\r\n0000000000 65535 f\r\n";
	for (size_t i = 0; i < objStarts.size(); i++) {
		char objStartStr[32];
		snprintf(objStartStr, sizeof(objStartStr), "%010u 00000 n\r\n", (unsigned int)objStarts[i]);
		pdfData += objStartStr;
	}
	pdfData += "trailer\r\n<</Root 5 0 R/Info 6 0 R/Size 6>>\r\nstartxref\r\n";
	char startxrefStr[16];
	snprintf(startxrefStr, sizeof(startxrefStr), "%u", (unsigned int)xrefStart);
	pdfData += startxrefStr;
	pdfData += "\r\n%%EOF\r\n";

	return pdfData;
}

int main(int argc, char* argv[]) {
	uint32_t a = 0, b = 0, c = 0, d  = 0;
	bool seed_set = false;
	output_format_t output_format = NORMAL;
	char* output_file = NULL;
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
		}
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
	NanpureBoard nb = generateNanpure(rng);
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
			fprintf(fp, "%s\n\n", seedString);
			for (int i = 0; i < 9; i++) {
				fprintf(fp, "%d", nb.getNumber(i, 0));
				for (int j = 1; j < 9; j++) {
					fprintf(fp, " %d", nb.getNumber(i, j));
				}
				fputc('\n', fp);
			}
			break;
		case ONELINE:
			fprintf(fp, "%s\n\n", seedString);
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					fprintf(fp, "%d", nb.getNumber(i, j));
				}
			}
			fputc('\n', fp);
			break;
		case PDF:
			{
				std::string pdfData = create_pdf(nb, seedString);
				fwrite(pdfData.data(), 1, pdfData.size(), fp);
			}
	}
	fclose(fp);
	return 0;
}
