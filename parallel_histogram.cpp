#include "parallel_histogram.h"
#include "histogram.h"

void run(taskInfo* ti) {
	if (ti != nullptr) {
		list<string>::iterator it = ti->rows->begin();
		advance(it, ti->startpos);
		for (int i = 0; i < ti->chuncksize; ++i) {
			calculate_statistics(*it, *(ti->statistics));
			it++;
		}
	}
}

/*
*  known issue:
*  - will not work with huge files. potential solution - split file processing to short cycles
*    in each cycle limit number of rows that are teken for processing
*/
array<int, num_letters> parallel_histogram(const std::string& file_path) {

	array<int, num_letters> statistics;
	init(statistics);
	array<array<int, num_letters>, num_workers> chunksStat;
	for (int i = 0; i < num_workers; ++i)
		init(chunksStat[i]);
	array<taskInfo*, num_workers> taskChunks;

	ifstream file(file_path);
	if (file.is_open()) {
		// init 
		list<string> rows;
		string line;
		while (getline(file, line)) {
			rows.emplace_back(line);
		}
		file.close();

		vector<thread> workersThreads;

		int chunkSize = rows.size() / num_workers;
		for (int n = 0; n < num_workers; ++n) {
			taskChunks[n] = new taskInfo(&rows, &chunksStat[n], n * chunkSize, (n < num_workers - 1) ? chunkSize : rows.size() - chunkSize * (num_workers - 1));
			workersThreads.emplace_back(run, taskChunks[n]);
		}

		for (auto& thread : workersThreads) {
			thread.join();
		}

		for (int i = 0; i < num_workers; ++i) {
			for (int n = 0; n < num_letters; ++n)
				statistics[n] += chunksStat[i][n];
		}

		for (int n = 0; n < num_workers; ++n)
			delete taskChunks[n];
	}
	else {
		cout << "Can't open file " << file_path << endl;
	}

	print_histogram(file_path, statistics);

	return statistics;
}
