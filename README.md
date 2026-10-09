# Sorting Experiment – IT003

## 1. Thông tin sinh viên
- Họ và tên: Đinh Thị Quỳnh Thư
- MSSV: 24521724
- Lớp: IT003.R17
- Trường: Đại học Công nghệ Thông tin – ĐHQG-HCM

## 2. Giới thiệu
Bài thực nghiệm triển khai và so sánh hiệu năng của bốn thuật toán sắp xếp trong C++:
- QuickSort
- HeapSort
- MergeSort
- `std::sort` (STL)

## 3. Cấu trúc dự án
- `src/benchmark.cpp`: Chương trình đo hiệu năng.
- `src/generate_data.cpp`: Chương trình tạo dữ liệu.
- `src/sorting.h`: Mã nguồn các thuật toán sắp xếp.
- `src/test_sorts.cpp`: Chương trình kiểm thử.
- `data/`: Các bộ dữ liệu thực nghiệm.
- `results/`: Kết quả đo thời gian và biểu đồ.
- `report/`: Báo cáo thực nghiệm.

## 4. Biên dịch và chạy
Yêu cầu trình biên dịch hỗ trợ C++17.

```bash
g++ -std=c++17 -O2 src/generate_data.cpp -o generate_data
g++ -std=c++17 -O2 src/test_sorts.cpp -o test_sorts
g++ -std=c++17 -O2 src/benchmark.cpp -o benchmark
```

Chạy các chương trình sau khi biên dịch:

```bash
./generate_data
./test_sorts
./benchmark
```

Lưu ý: Các lệnh chạy trên cần được kiểm tra với mã nguồn và tham số thực tế của chương trình.

## 5. Kết quả thực nghiệm
Dữ liệu kết quả và biểu đồ được lưu trong thư mục `results/`. Báo cáo chi tiết nằm trong thư mục `report/`.
