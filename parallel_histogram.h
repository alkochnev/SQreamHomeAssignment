#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <list>
#include <array>
#include <string>
#include "histogram.h"

using namespace std;

typedef struct taskInfo {
	list<string>* rows;
	array<int, num_letters>* statistics;
	int startpos;
	int chuncksize;

	taskInfo(list<string>* r, array<int, num_letters>* s, int sp, int cs) {
		rows = r; statistics = s; startpos = sp; chuncksize = cs;
	}
} taskInfo;

void run(taskInfo* ti);

array<int, num_letters> parallel_histogram(const std::string& file_path);
