// FileSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>

std::mutex mtx;

void writeToFile(const std::string& filename, const std::string& data) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << data << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
    }
}

void readFromFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file for reading: " << filename << std::endl;
    }
}

int main() {
    const std::string filename = "example.txt";

    // Создаем потоки для записи и чтения файла
    std::thread writer([&filename]() {
        for (int i = 0; i < 5; ++i) {
            writeToFile(filename, "Writing line " + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        });

    std::thread reader([&filename]() {
        for (int i = 0; i < 5; ++i) {
            readFromFile(filename);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        });

    // Ждем завершения работы потоков
    writer.join();
    reader.join();

    return 0;
}