#include "histogram.h"

void init(array<int, num_letters>& statistics) {
	for (int n = 0; n < num_letters; ++n)
		statistics[n] = 0;
}

void calculate_statistics(string line, array<int, num_letters>& statistics) {
	for (auto c : line) {
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			++statistics[(c >= 'a') ? (c - 'a') : (c - 'A')];
		}
	}
}

void print_histogram(string fname, array<int, num_letters>& statistics) {
	cout << "File " << fname << " contains following letters histogram: " << endl;
	for (int n = 0; n < num_letters; ++n) {
		if (statistics[n] > 0)
			cout << (char)('a' + n) << " = " << setw(4) << statistics[n] << endl;
	}
}

array<int, num_letters> histogram(const std::string& file_path) {

	array<int, num_letters> statistics;
	init(statistics);

	ifstream file(file_path);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			calculate_statistics(line, statistics);
		}
		file.close();
	}
	else {
		cout << "Can't open file " << file_path << endl;
	}

	print_histogram(file_path, statistics);

	return statistics;
}

