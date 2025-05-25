#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <functional>

class StringSortTester {
public:
  using SortFunction = std::function<void(std::vector<std::string>&, int&)>;

  static void standardQuickSort(std::vector<std::string>& arr, int& comparisons);
  static void standardMergeSort(std::vector<std::string>& arr, int& comparisons);
  static void stringQuickSort(std::vector<std::string>& arr, int& comparisons);
  static void stringMergeSort(std::vector<std::string>& arr, int& comparisons);
  static void msdRadixSort(std::vector<std::string>& arr, int& comparisons);
  static void msdRadixQuickSort(std::vector<std::string>& arr, int& comparisons);

  static std::pair<std::chrono::nanoseconds, int> testAlgorithm(
    SortFunction algorithm,
    std::vector<std::string> arr,
    int runs = 5
  );

private:
  static int lcp(const std::string& a, const std::string& b);
  static bool lcpCompare(const std::string& a, const std::string& b, int lcp_len, int& comparisons);

  static void merge(std::vector<std::string>& arr, int left, int mid, int right, int& comparisons, bool use_lcp);
  static void mergeSort(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp);
  static void lcpMergeSort(std::vector<std::string>& arr, int left, int right, int& comparisons);

  static int partition(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp);
  static void quickSort(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp);

  static int compare(const std::string& a, const std::string& b, int d, int& comparisons);
  static void ternaryQuickSort(std::vector<std::string>& arr, int left, int right, int d, int& comparisons);

  static void msdRadix(std::vector<std::string>& arr, int low, int high, int d, int& comparisons, bool use_quick);
};