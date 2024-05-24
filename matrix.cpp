#include <iostream>
#include <vector>
#include <thread>

// Функция для перемножения двух матриц
void multiplyMatrices(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2, std::vector<std::vector<int>>& result, int rowStart, int rowEnd) {
    for (int i = rowStart; i < rowEnd; ++i) {
        for (int j = 0; j < matrix2[0].size(); ++j) {
            for (int k = 0; k < matrix1[0].size(); ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main() {
    std::vector<std::vector<int>> matrix1 = { {1, 2}, {3, 4} };
    std::vector<std::vector<int>> matrix2 = { {5, 6}, {7, 8} };
    std::vector<std::vector<int>> result(matrix1.size(), std::vector<int>(matrix2[0].size(), 0));

    // Однопоточный подход
    multiplyMatrices(matrix1, matrix2, result, 0, matrix1.size());

    // Вывод результата перемножения
    std::cout << "Результат перемножения матриц:\n";
    for (const auto& row : result) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    // Многопоточный подход
    int numThreads = 4; // Количество потоков
    std::vector<std::thread> threads;
    int rowsPerThread = matrix1.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * rowsPerThread;
        int end = (i == numThreads - 1) ? matrix1.size() : (i + 1) * rowsPerThread;
        threads.emplace_back(multiplyMatrices, std::ref(matrix1), std::ref(matrix2), std::ref(result), start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Вывод результата перемножения
    std::cout << "Результат перемножения матриц в многопоточном режиме:\n";
    for (const auto& row : result) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}