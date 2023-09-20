#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <list>
#include <array>
#include <string>

using namespace std;

constexpr int num_letters = 26;
constexpr int num_workers = 10;

void init(array<int, num_letters>& statistics);

void calculate_statistics(string line, array<int, num_letters>& statistics);

void print_histogram(string fname, array<int, num_letters>& statistics);

array<int, num_letters> histogram(const std::string& file_path);