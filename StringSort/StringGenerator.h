#pragma once
#include <vector>
#include <string>
#include <random>

class StringGenerator {
public:
  StringGenerator();
  std::string generate_random_string(int length);
  std::vector<std::string> generate_random_array(int size, int min_len = 10, int max_len = 200);
  std::vector<std::string> generate_reverse_sorted_array(int size, int min_len = 10, int max_len = 200);
  std::vector<std::string> generate_almost_sorted_array(int size, int min_len = 10, int max_len = 200, int swaps = 10);
  std::vector<std::string> generate_common_prefix_array(int size, int prefix_len = 5, int min_len = 10, int max_len = 200);
  void save_to_file(const std::vector<std::string>& arr, const std::string& filename);
  std::vector<std::string> load_from_file(const std::string& filename);

private:
  std::vector<char> chars;
  std::mt19937 rng;
};