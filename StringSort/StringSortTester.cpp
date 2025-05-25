#include "StringSortTester.h"
#include <algorithm>
#include <fstream>

int StringSortTester::lcp(const std::string& a, const std::string& b) {
  int len = 0;
  int min_len = std::min(a.size(), b.size());
  while (len < min_len && a[len] == b[len]) {
    len++;
  }
  return len;
}

bool StringSortTester::lcpCompare(const std::string& a, const std::string& b, int lcp_len, int& comparisons) {
  comparisons++;
  if (a.size() == lcp_len && b.size() == lcp_len) return false;
  if (a.size() == lcp_len) return true;
  if (b.size() == lcp_len) return false;
  return a[lcp_len] < b[lcp_len];
}

void StringSortTester::merge(std::vector<std::string>& arr, int left, int mid, int right, int& comparisons, bool use_lcp) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  std::vector<std::string> L(arr.begin() + left, arr.begin() + mid + 1);
  std::vector<std::string> R(arr.begin() + mid + 1, arr.begin() + right + 1);

  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    if (use_lcp) {
      int lcp_len = lcp(L[i], R[j]);
      if (lcpCompare(L[i], R[j], lcp_len, comparisons)) {
        arr[k++] = L[i++];
      }
      else {
        arr[k++] = R[j++];
      }
    }
    else {
      comparisons++;
      if (L[i] < R[j]) {
        arr[k++] = L[i++];
      }
      else {
        arr[k++] = R[j++];
      }
    }
  }

  while (i < n1) arr[k++] = L[i++];
  while (j < n2) arr[k++] = R[j++];
}

void StringSortTester::mergeSort(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, comparisons, use_lcp);
    mergeSort(arr, mid + 1, right, comparisons, use_lcp);
    merge(arr, left, mid, right, comparisons, use_lcp);
  }
}

void StringSortTester::lcpMergeSort(std::vector<std::string>& arr, int left, int right, int& comparisons) {
  mergeSort(arr, left, right, comparisons, true);
}

int StringSortTester::partition(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp) {
  std::string pivot = arr[right];
  int i = left - 1;

  for (int j = left; j < right; ++j) {
    bool less;
    if (use_lcp) {
      int lcp_len = lcp(arr[j], pivot);
      less = lcpCompare(arr[j], pivot, lcp_len, comparisons);
    }
    else {
      comparisons++;
      less = arr[j] < pivot;
    }

    if (less) {
      swap(arr[++i], arr[j]);
    }
  }

  swap(arr[i + 1], arr[right]);
  return i + 1;
}

void StringSortTester::quickSort(std::vector<std::string>& arr, int left, int right, int& comparisons, bool use_lcp) {
  if (left < right) {
    int pi = partition(arr, left, right, comparisons, use_lcp);
    quickSort(arr, left, pi - 1, comparisons, use_lcp);
    quickSort(arr, pi + 1, right, comparisons, use_lcp);
  }
}

int StringSortTester::compare(const std::string& a, const std::string& b, int d, int& comparisons) {
  comparisons++;
  if (d < a.size() && d < b.size()) {
    if (a[d] < b[d]) return -1;
    if (a[d] > b[d]) return 1;
    return 0;
  }
  if (d < a.size()) return 1;
  if (d < b.size()) return -1;
  return 0;
}

void StringSortTester::ternaryQuickSort(std::vector<std::string>& arr, int left, int right, int d, int& comparisons) {
  if (left >= right) return;

  int mid = left + (right - left) / 2;
  if (compare(arr[right], arr[left], d, comparisons) < 0) swap(arr[left], arr[right]);
  if (compare(arr[mid], arr[left], d, comparisons) < 0) swap(arr[mid], arr[left]);
  if (compare(arr[right], arr[mid], d, comparisons) < 0) swap(arr[mid], arr[right]);

  std::string pivot_str = arr[mid];
  swap(arr[left], arr[mid]);

  int lt = left, gt = right;
  int i = left + 1;

  while (i <= gt) {
    int cmp = compare(arr[i], pivot_str, d, comparisons);
    if (cmp < 0) {
      swap(arr[lt++], arr[i++]);
    }
    else if (cmp > 0) {
      swap(arr[i], arr[gt--]);
    }
    else {
      i++;
    }
  }

  ternaryQuickSort(arr, left, lt - 1, d, comparisons);
  if (d < pivot_str.size()) {
    ternaryQuickSort(arr, lt, gt, d + 1, comparisons);
  }
  ternaryQuickSort(arr, gt + 1, right, d, comparisons);
}

void StringSortTester::msdRadix(std::vector<std::string>& arr, int low, int high, int d, int& comparisons, bool use_quick) {
  if (low >= high) return;

  if (use_quick && (high - low + 1 < 74)) {
    ternaryQuickSort(arr, low, high, d, comparisons);
    return;
  }

  const int ALPHABET_SIZE = 256;
  std::vector<int> count(ALPHABET_SIZE + 2, 0);
  std::vector<std::string> temp(high - low + 1);

  for (int i = low; i <= high; ++i) {
    int c = (d < arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
    count[c]++;
    comparisons++;
  }

  for (int i = 1; i < count.size(); ++i) {
    count[i] += count[i - 1];
  }

  for (int i = high; i >= low; --i) {
    int c = (d < arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
    temp[--count[c]] = arr[i];
  }

  for (int i = low; i <= high; ++i) {
    arr[i] = temp[i - low];
  }

  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    msdRadix(arr, low + count[i], low + count[i + 1] - 1, d + 1, comparisons, use_quick);
  }
}

void StringSortTester::standardQuickSort(std::vector<std::string>& arr, int& comparisons) {
  quickSort(arr, 0, static_cast<int>(arr.size()) - 1, comparisons, false);
}

void StringSortTester::standardMergeSort(std::vector<std::string>& arr, int& comparisons) {
  mergeSort(arr, 0, static_cast<int>(arr.size()) - 1, comparisons, false);
}

void StringSortTester::stringQuickSort(std::vector<std::string>& arr, int& comparisons) {
  ternaryQuickSort(arr, 0, static_cast<int>(arr.size()) - 1, 0, comparisons);
}

void StringSortTester::stringMergeSort(std::vector<std::string>& arr, int& comparisons) {
  lcpMergeSort(arr, 0, static_cast<int>(arr.size()) - 1, comparisons);
}

void StringSortTester::msdRadixSort(std::vector<std::string>& arr, int& comparisons) {
  msdRadix(arr, 0, static_cast<int>(arr.size()) - 1, 0, comparisons, false);
}

void StringSortTester::msdRadixQuickSort(std::vector<std::string>& arr, int& comparisons) {
  msdRadix(arr, 0, static_cast<int>(arr.size()) - 1, 0, comparisons, true);
}

std::pair<std::chrono::nanoseconds, int> StringSortTester::testAlgorithm(
  SortFunction algorithm,
  std::vector<std::string> arr,
  int runs
) {
  int total_comparisons = 0;
  auto total_time = std::chrono::nanoseconds(0);

  for (int i = 0; i < runs; ++i) {
    std::vector<std::string> copy = arr;
    int comparisons = 0;

    auto start = std::chrono::high_resolution_clock::now();
    algorithm(copy, comparisons);
    auto end = std::chrono::high_resolution_clock::now();

    total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    total_comparisons += comparisons;

    if (!is_sorted(copy.begin(), copy.end())) {
      throw std::runtime_error("Sorting failed!");
    }
  }

  return {
      total_time / runs,
      total_comparisons / runs
  };
}