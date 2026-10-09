#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

constexpr std::size_t N = 1'000'000;
constexpr unsigned BASE_SEED = 20261009;

// Doc lai file de kiem tra du lieu thuc su da luu.
void verifyFile(const std::string& path, int id) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Khong mo duoc: " + path);
    }

    std::size_t count = 0;
    double value = 0;
    double previous = 0;
    bool ascending = true;
    bool descending = true;

    while (input >> value) {
        if (!std::isfinite(value) ||
            value < -1'000'000.0 || value > 1'000'000.0) {
            throw std::runtime_error("Gia tri khong hop le: " + path);
        }

        if (count > 0) {
            if (value < previous) ascending = false;
            if (value > previous) descending = false;
        }

        previous = value;
        ++count;
    }

    if (!input.eof() || count != N) {
        throw std::runtime_error("Loi noi dung/so luong: " + path);
    }

    if (id == 1 && !ascending) {
        throw std::runtime_error("Day 1 chua tang dan");
    }
    if (id == 2 && !descending) {
        throw std::runtime_error("Day 2 chua giam dan");
    }
    if (id >= 3 && (ascending || descending)) {
        throw std::runtime_error("Day ngau nhien lai co thu tu");
    }

    std::cout << path << " | " << count
              << " phan tu | KIEM TRA OK\n";
}

int main() {
    try {
        std::ofstream manifest("data/manifest.csv");
        if (!manifest) {
            throw std::runtime_error("Khong ghi duoc manifest.csv");
        }

        manifest << "dataset,file,count,order,seed,min,max,decimal_places\n";

        for (int id = 1; id <= 10; ++id) {
            const unsigned seed = BASE_SEED + id;
            std::mt19937 generator(seed);
            std::uniform_int_distribution<int> distribution(
                -1'000'000'000, 1'000'000'000
            );

            // Chi giu mot day trong RAM tai mot thoi diem.
            std::vector<double> values(N);
            for (double& value : values) {
                value = distribution(generator) / 1000.0;
            }

            std::string order = "random";

            if (id == 1) {
                std::sort(values.begin(), values.end());
                order = "ascending";
            } else if (id == 2) {
                std::sort(values.begin(), values.end());
                std::reverse(values.begin(), values.end());
                order = "descending";
            }

            const std::string number =
                (id < 10 ? "0" : "") + std::to_string(id);
            const std::string path =
                "data/dataset_" + number + ".txt";

            std::ofstream output(path);
            if (!output) {
                throw std::runtime_error("Khong tao duoc: " + path);
            }

            output << std::fixed << std::setprecision(3);
            for (double value : values) {
                output << value << '\n';
            }

            output.close();
            if (!output) {
                throw std::runtime_error("Ghi file that bai: " + path);
            }

            verifyFile(path, id);

            manifest << id << ',' << path << ',' << N << ','
                     << order << ',' << seed
                     << ",-1000000,1000000,3\n";
        }

        manifest.close();
        if (!manifest) {
            throw std::runtime_error("Ghi manifest that bai");
        }

        std::cout << "HOAN TAT: 10 bo du lieu hop le.\n";
    } catch (const std::exception& error) {
        std::cerr << "LOI: " << error.what() << '\n';
        return 1;
    }

    return 0;
}