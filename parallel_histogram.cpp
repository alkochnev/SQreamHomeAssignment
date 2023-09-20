#include "parallel_histogram.h"
#include "histogram.h"

void run(taskInfo*  ti) {
	if (ti != nullptr) {
		// get iterator
		list<string>::iterator it = ti->rows->begin();
		// move to relevant position
		advance(it, ti->startpos);
		// calculate
		for (int i = 0; i < ti->chuncksize && it != ti->rows->end(); ++i) {
			calculate_statistics(*it, *(ti->statistics));
			it++;
		}
	}
}

void runInParallel(list<string> rows, array<int, num_letters>& statistics) {
	vector<thread> workersThreads;
	array<array<int, num_letters>, num_workers> chunksStat;
	for (int i = 0; i < num_workers; ++i)
		init(chunksStat[i]);
	array<unique_ptr<taskInfo>, num_workers> taskChunks;

	// init threads' pool for parallel processing
	size_t chunkSize = rows.size() / num_workers;
	for (size_t n = 0; n < num_workers; ++n) {
		unique_ptr<taskInfo> tmp(new taskInfo(&rows, &chunksStat[n], n * chunkSize, (n < num_workers - 1) ? chunkSize : rows.size() - chunkSize * (num_workers - 1)));
		taskChunks[n] = move(tmp);
		workersThreads.emplace_back(run, taskChunks[n].get());
	}
	// wait for all thread finished
	for (auto& thread : workersThreads) {
		thread.join();
	}
	// aggregate results
	for (int i = 0; i < num_workers; ++i) {
		for (int n = 0; n < num_letters; ++n)
			statistics[n] += chunksStat[i][n];
	}
}

array<int, num_letters> parallel_histogram(const std::string& file_path) {

	array<int, num_letters> statistics;
	init(statistics);

	ifstream file(file_path);
	if (file.is_open()) {
		// init 
		list<string> rows;
		string line;
		int rowCounter = 0;
		// get rows portion
		while (getline(file, line)) {
			rows.emplace_back(line);
			++rowCounter;
			// if read enough rows from file, start processing
			if (rowCounter == 1000) {
				runInParallel(rows, statistics);
				rows.clear();
				rowCounter = 0;
			}
		}
		if (rowCounter > 0)
			runInParallel(rows, statistics);

		file.close();



	}
	else {
		cout << "Can't open file " << file_path << endl;
	}

	// print results
	print_histogram(file_path, statistics);

	return statistics;
}
