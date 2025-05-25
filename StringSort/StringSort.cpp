#include "StringGenerator.h"
#include "StringSortTester.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void run_experiment() {
  StringGenerator generator;
  const int MAX_SIZE = 3000;
  const int STEP = 100;
  const int RUNS = 5;

  if (!fs::exists("results")) {
    fs::create_directory("results");
  }

  auto random_data = generator.generate_random_array(MAX_SIZE);
  auto reverse_data = generator.generate_reverse_sorted_array(MAX_SIZE);
  auto almost_data = generator.generate_almost_sorted_array(MAX_SIZE);
  auto prefix_data = generator.generate_common_prefix_array(MAX_SIZE);

  generator.save_to_file(random_data, "results/random.txt");
  generator.save_to_file(reverse_data, "results/reverse.txt");
  generator.save_to_file(almost_data, "results/almost.txt");
  generator.save_to_file(prefix_data, "results/prefix.txt");

  std::vector<std::pair<std::string, StringSortTester::SortFunction>> algorithms = {
      {"Standard QuickSort", StringSortTester::standardQuickSort},
      {"Standard MergeSort", StringSortTester::standardMergeSort},
      {"String QuickSort", StringSortTester::stringQuickSort},
      {"String MergeSort", StringSortTester::stringMergeSort},
      {"MSD RadixSort", StringSortTester::msdRadixSort},
      {"MSD Radix+QuickSort", StringSortTester::msdRadixQuickSort}
  };

  std::vector<std::pair<std::string, std::vector<std::string>>> datasets = {
      {"Random", random_data},
      {"Reverse Sorted", reverse_data},
      {"Almost Sorted", almost_data},
      {"Common Prefix", prefix_data}
  };

  std::ofstream results("results/results.csv");
  results << "Algorithm,Data Size,Data Type,Time (ns),Comparisons\n";

  for (const auto& dataset : datasets) {
    const std::string& data_name = dataset.first;
    const std::vector<std::string>& data = dataset.second;

    std::cout << "Testing on " << data_name << " data...\n";

    for (int size = STEP; size <= MAX_SIZE; size += STEP) {
      std::vector<std::string> test_data(data.begin(), data.begin() + size);

      for (const auto& algorithm : algorithms) {
        const std::string& algo_name = algorithm.first;
        const StringSortTester::SortFunction& algo_func = algorithm.second;

        try {
          auto result = StringSortTester::testAlgorithm(algo_func, test_data, RUNS);
          auto time = result.first;
          auto comparisons = result.second;

          results << algo_name << "," << size << "," << data_name << ","
            << time.count() << "," << comparisons << "\n";

          std::cout << algo_name << " on " << data_name << " (" << size << "): "
            << time.count() << " ns, " << comparisons << " comparisons\n";
        }
        catch (const std::exception& e) {
          std::cerr << "Error with " << algo_name << " on " << data_name
            << " (" << size << "): " << e.what() << "\n";
        }
      }
    }
  }

  std::cout << "Experiments completed. Results saved to results/results.csv\n";
}

int main() {
  try {
    run_experiment();
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}