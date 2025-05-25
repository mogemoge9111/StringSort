#include "StringGenerator.h"
#include <fstream>
#include <algorithm>

StringGenerator::StringGenerator() : rng(std::random_device{}()) {
  for (char c = 'A'; c <= 'Z'; ++c) chars.push_back(c);
  for (char c = 'a'; c <= 'z'; ++c) chars.push_back(c);
  for (char c = '0'; c <= '9'; ++c) chars.push_back(c);
  std::string special = "!@#%:;^&*()-.";
  for (char c : special) chars.push_back(c);
}

std::string StringGenerator::generate_random_string(int length) {
  std::string result;
  std::uniform_int_distribution<int> dist(0, static_cast<int>(chars.size()) - 1);
  for (int i = 0; i < length; ++i) {
    result += chars[dist(rng)];
  }
  return result;
}

std::vector<std::string> StringGenerator::generate_random_array(int size, int min_len, int max_len) {
  std::vector<std::string> arr;
  std::uniform_int_distribution<int> len_dist(min_len, max_len);
  for (int i = 0; i < size; ++i) {
    arr.push_back(generate_random_string(len_dist(rng)));
  }
  return arr;
}

std::vector<std::string> StringGenerator::generate_reverse_sorted_array(int size, int min_len, int max_len) {
  auto arr = generate_random_array(size, min_len, max_len);
  std::sort(arr.begin(), arr.end(), std::greater<std::string>());
  return arr;
}

std::vector<std::string> StringGenerator::generate_almost_sorted_array(int size, int min_len, int max_len, int swaps) {
  auto arr = generate_random_array(size, min_len, max_len);
  std::sort(arr.begin(), arr.end());

  std::uniform_int_distribution<int> index_dist(0, size - 1);
  for (int i = 0; i < swaps; ++i) {
    int a = index_dist(rng);
    int b = index_dist(rng);
    std::swap(arr[a], arr[b]);
  }
  return arr;
}

std::vector<std::string> StringGenerator::generate_common_prefix_array(int size, int prefix_len, int min_len, int max_len) {
  std::vector<std::string> arr;
  std::string common_prefix = generate_random_string(prefix_len);
  std::uniform_int_distribution<int> len_dist(min_len, max_len);

  for (int i = 0; i < size; ++i) {
    int len = len_dist(rng);
    std::string suffix;
    if (len > prefix_len) {
      suffix = generate_random_string(len - prefix_len);
    }
    arr.push_back(common_prefix + suffix);
  }
  return arr;
}

void StringGenerator::save_to_file(const std::vector<std::string>& arr, const std::string& filename) {
  std::ofstream out(filename);
  out << arr.size() << std::endl;
  for (const std::string& s : arr) {
    out << s << std::endl;
  }
}

std::vector<std::string> StringGenerator::load_from_file(const std::string& filename) {
  std::ifstream in(filename);
  int size;
  in >> size;
  std::vector<std::string> arr(size);
  for (int i = 0; i < size; ++i) {
    in >> arr[i];
  }
  return arr;
}