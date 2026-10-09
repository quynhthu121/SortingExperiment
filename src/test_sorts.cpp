#include "sorting.hpp"
#include <iostream>
#include <random>
#include <vector>

using SortFunction = void (*)(std::vector<double>&);

struct Algorithm {
    const char* name;
    SortFunction sort;
};

struct TestCase {
    const char* name;
    std::vector<double> input;
};

bool checkSort(
    const Algorithm& algorithm,
    const TestCase& test
) {
    auto expected = test.input;
    std::sort(expected.begin(), expected.end());

    // Moi thuat toan nhan mot ban sao moi.
    auto actual = test.input;
    algorithm.sort(actual);

    bool passed =
        std::is_sorted(actual.begin(), actual.end()) &&
        actual == expected;

    std::cout << test.name << ": "
              << (passed ? "PASS" : "FAIL") << '\n';

    return passed;
}

int main() {
    const Algorithm algorithms[] = {
    {"QuickSort", quickSort},
    {"HeapSort", heapSort},
    {"MergeSort", mergeSort},
    {"std::sort", cppSort}
};

    std::vector<TestCase> tests = {
        {"Rong", {}},
        {"Mot phan tu", {2.5}},
        {"Tang dan", {1, 2, 3, 4, 5}},
        {"Giam dan", {5, 4, 3, 2, 1}},
        {"Trung", {3, 1, 3, 2, 1, 3}},
        {"Tat ca bang nhau", {7, 7, 7, 7}},
        {"Am va thap phan", {-2.5, 0, -8.1, 3.2, -2.5}}
    };

    std::mt19937 generator(42);
    std::uniform_int_distribution<int> distribution(-100, 100);

    std::vector<double> randomData(10'000);
    for (double& value : randomData) {
        value = distribution(generator) / 10.0;
    }

    tests.push_back({"Ngau nhien 10000", randomData});

    bool allPassed = true;

    for (const auto& algorithm : algorithms) {
        std::cout << "\n=== " << algorithm.name << " ===\n";

        for (const auto& test : tests) {
            allPassed &= checkSort(algorithm, test);
        }
    }

    std::cout << (allPassed
        ? "\nTAT CA THUAT TOAN: PASS\n"
        : "\nCO LOI CAN SUA\n");

    return allPassed ? 0 : 1;
}