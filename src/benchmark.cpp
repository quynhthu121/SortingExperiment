#include "sorting.hpp"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr std::size_t N = 1'000'000;
using SortFunction = void (*)(std::vector<double>&);

struct Algorithm {
    const char* key;
    const char* name;
    SortFunction sort;
};

const Algorithm algorithms[] = {
    {"quick", "QuickSort", quickSort},
    {"heap", "HeapSort", heapSort},
    {"merge", "MergeSort", mergeSort},
    {"std", "std::sort", cppSort}
};

// Doc va kiem tra mot bo du lieu.
std::vector<double> loadData(const std::string& path, int dataset) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Khong mo duoc: " + path);
    }

    std::vector<double> data(N);
    for (double& value : data) {
        if (!(input >> value)) {
            throw std::runtime_error(
                "Thieu phan tu hoac sai dinh dang: " + path
            );
        }

        if (!std::isfinite(value) || value < -1'000'000.0 ||
            value > 1'000'000.0) {
            throw std::runtime_error("Gia tri khong hop le: " + path);
        }
    }

    // Sau N phan tu, chi duoc con khoang trang.
    input >> std::ws;
    if (input.bad() || input.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error("Du phan tu hoac du ky tu: " + path);
    }

    const bool ascending = std::is_sorted(data.begin(), data.end());
    const bool descending = std::is_sorted(
        data.begin(), data.end(), std::greater<double>{}
    );

    if ((dataset == 1 && !ascending) ||
        (dataset == 2 && !descending) ||
        (dataset >= 3 && (ascending || descending))) {
        throw std::runtime_error("Sai thu tu du lieu: " + path);
    }

    return data;
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 2 && std::string(argv[1]) == "--help") {
            std::cout
                << "Cach dung: benchmark.exe [all|quick|heap|merge|std]\n"
                << "Khong truyen tham so: chay ca 4 thuat toan.\n"
                << "Moi thuat toan chay 1 lan tren moi bo du lieu.\n";
            return 0;
        }

        if (argc > 2) {
            throw std::runtime_error("Qua nhieu tham so.");
        }

        const std::string selected = argc == 2 ? argv[1] : "all";
        bool valid = selected == "all";

        for (const auto& algorithm : algorithms) {
            if (selected == algorithm.key) {
                valid = true;
            }
        }

        if (!valid) {
            throw std::runtime_error(
                "Tham so khong hop le. Dung --help."
            );
        }

        std::filesystem::create_directories("results");

        const std::string outputPath = selected == "all"
            ? "results/benchmark.csv"
            : "results/benchmark_" + selected + ".csv";

        // Bao ve ket qua cu khoi bi ghi de.
        if (std::filesystem::exists(outputPath)) {
            throw std::runtime_error(
                "File da ton tai, hay luu ban cu truoc: " + outputPath
            );
        }

        std::ofstream csv;
        csv.exceptions(std::ios::failbit | std::ios::badbit);
        csv.open(outputPath);

        csv << "dataset,order,algorithm,run,execution_order,"
               "count,time_ms,verified\n";
        csv << std::fixed << std::setprecision(6);
        std::cout << std::fixed << std::setprecision(3);

        int measurements = 0;

        for (int dataset = 1; dataset <= 10; ++dataset) {
            std::ostringstream path;
            path << "data/dataset_" << std::setfill('0')
                 << std::setw(2) << dataset << ".txt";

            const auto original = loadData(path.str(), dataset);

            // Tao ket qua chuan NGOAI khoang do.
            auto expected = original;
            std::sort(expected.begin(), expected.end());

            const char* order = dataset == 1 ? "ascending"
                : dataset == 2 ? "descending" : "random";

            int executionOrder = 0;

            // Luan phien thuat toan bat dau giua cac bo du lieu.
            for (int offset = 0; offset < 4; ++offset) {
                const auto& algorithm =
                    algorithms[(dataset - 1 + offset) % 4];

                if (selected != "all" && selected != algorithm.key) {
                    continue;
                }

                ++executionOrder;

                // Moi lan sap xep nhan ban sao moi.
                auto actual = original;

                // CHI DO LOI GOI SAP XEP.
                const auto start = std::chrono::steady_clock::now();
                algorithm.sort(actual);
                const auto finish = std::chrono::steady_clock::now();

                const double ms =
                    std::chrono::duration<double, std::milli>(
                        finish - start
                    ).count();

                // Kiem tra NGOAI khoang do.
                if (!std::is_sorted(actual.begin(), actual.end()) ||
                    actual != expected) {
                    throw std::runtime_error(
                        std::string("Sap xep sai: ") + algorithm.name +
                        ", dataset " + std::to_string(dataset)
                    );
                }

                // Ghi file va in man hinh NGOAI khoang do.
                csv << dataset << ',' << order << ',' << algorithm.name
                    << ",1," << executionOrder << ',' << N << ','
                    << ms << ",PASS\n";

                std::cout << "Dataset " << dataset
                          << " | " << algorithm.name
                          << " | " << ms << " ms | KIEM TRA OK\n";

                ++measurements;
            }
        }

        csv.close();

        std::cout << "HOAN THANH: " << measurements << " phep do\n"
                  << "CSV: " << outputPath << '\n';

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "LOI: " << error.what() << '\n';
        return 1;
    }
}