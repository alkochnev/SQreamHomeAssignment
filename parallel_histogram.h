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
	size_t startpos;
	size_t chuncksize;

	taskInfo(list<string>* r, array<int, num_letters>* s, size_t sp, size_t cs) {
		rows = r; statistics = s; startpos = sp; chuncksize = cs;
	}

	taskInfo(taskInfo&& o) {
		rows = o.rows;
		statistics = o.statistics;
		startpos = o.startpos;
		chuncksize = o.chuncksize;
		o.rows = nullptr;
		o.statistics = nullptr;
	}

	taskInfo(taskInfo& o) {
		rows = o.rows;
		statistics = o.statistics;
		startpos = o.startpos;
		chuncksize = o.chuncksize;
	}
} taskInfo;

void run(taskInfo* ti);

array<int, num_letters> parallel_histogram(const std::string& file_path);
