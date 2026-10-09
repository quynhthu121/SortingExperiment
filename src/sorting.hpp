#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

// Lay gia tri o giua trong ba gia tri.
inline double medianOfThree(double a, double b, double c) {
    if (a > b) std::swap(a, b);
    if (b > c) std::swap(b, c);
    if (a > b) std::swap(a, b);
    return b;
}

inline void quickSortRange(
    std::vector<double>& a,
    std::ptrdiff_t left,
    std::ptrdiff_t right
) {
    while (left < right) {
        double pivot = medianOfThree(
            a[left],
            a[left + (right - left) / 2],
            a[right]
        );

        std::ptrdiff_t i = left;
        std::ptrdiff_t j = right;

        // Phan hoach bang hai con tro.
        while (i <= j) {
            while (i <= right && a[i] < pivot) ++i;
            while (j >= left && a[j] > pivot) --j;

            if (i <= j) {
                std::swap(a[i], a[j]);
                ++i;
                --j;
            }
        }

        // De quy phan nho; lap de xu ly phan lon.
        if (j - left < right - i) {
            quickSortRange(a, left, j);
            left = i;
        } else {
            quickSortRange(a, i, right);
            right = j;
        }
    }
}

inline void quickSort(std::vector<double>& a) {
    if (!a.empty()) {
        quickSortRange(
            a, 0,
            static_cast<std::ptrdiff_t>(a.size()) - 1
        );
    }
}
// Dua phan tu tai root xuong de khoi phuc max-heap.
// count la so phan tu thuoc heap hien tai.
inline void siftDown(
    std::vector<double>& a,
    std::size_t root,
    std::size_t count
) {
    while (root < count / 2) {
        std::size_t child = 2 * root + 1;

        // Chon nut con co gia tri lon hon.
        if (child + 1 < count && a[child] < a[child + 1]) {
            ++child;
        }

        if (a[root] >= a[child]) {
            break;
        }

        std::swap(a[root], a[child]);
        root = child;
    }
}

inline void heapSort(std::vector<double>& a) {
    const std::size_t n = a.size();

    // Xay dung max-heap tu duoi len.
    for (std::size_t start = n / 2; start > 0; --start) {
        siftDown(a, start - 1, n);
    }

    // Dua phan tu lon nhat ve cuoi, roi thu hep heap.
    for (std::size_t end = n; end > 1; --end) {
        std::swap(a[0], a[end - 1]);
        siftDown(a, 0, end - 1);
    }
}
// Sap xep doan [left, right): gom left, khong gom right.
inline void mergeSortRange(
    std::vector<double>& a,
    std::vector<double>& buffer,
    std::size_t left,
    std::size_t right
) {
    if (right - left < 2) {
        return;
    }

    const std::size_t middle = left + (right - left) / 2;

    // Sap xep hai nua.
    mergeSortRange(a, buffer, left, middle);
    mergeSortRange(a, buffer, middle, right);

    // Tron hai nua vao mang phu.
    std::size_t i = left;
    std::size_t j = middle;
    std::size_t k = left;

    while (i < middle && j < right) {
        // Khi bang nhau, lay phan tu ben trai truoc.
        if (a[i] <= a[j]) {
            buffer[k++] = a[i++];
        } else {
            buffer[k++] = a[j++];
        }
    }

    while (i < middle) {
        buffer[k++] = a[i++];
    }

    while (j < right) {
        buffer[k++] = a[j++];
    }

    // Chep doan da tron ve mang ban dau.
    for (std::size_t p = left; p < right; ++p) {
        a[p] = buffer[p];
    }
}

inline void mergeSort(std::vector<double>& a) {
    if (a.size() < 2) {
        return;
    }

    // Cap phat mot mang phu, dung chung cho cac lan tron.
    std::vector<double> buffer(a.size());
    mergeSortRange(a, buffer, 0, a.size());
}

// Phuong phap thu tu: goi ham sap xep cua thu vien C++.
inline void cppSort(std::vector<double>& a) {
    std::sort(a.begin(), a.end());
}