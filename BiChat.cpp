// helloworld.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

const char* SEMAPHORE_NAME = "/chat_semaphore";
const char* CHAT_FILE = "/tmp/chat_messages.txt";

int main() {
    sem_t* semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);

    // Создаем общую память для чата
    int shared_memory_fd = shm_open("/chat_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shared_memory_fd, 4096);
    void* chat_memory = mmap(0, 4096, PROT_WRITE | PROT_READ, MAP_SHARED, shared_memory_fd, 0);

    std::string message;

    while (true) {
        // Пользователь вводит сообщение
        std::cout << "Введите ваше сообщение ('exit' для выхода): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        sem_wait(semaphore); // Ждем, пока семафор станет доступным

        // Записываем сообщение в общую память
        sprintf((char*)chat_memory, "User: %s", message.c_str());

        sem_post(semaphore); // Освобождаем семафор
    }

    // Читаем сообщения из общей памяти и выводим их на экран
    while (true) {
        sem_wait(semaphore); // Ждем, пока семафор станет доступным

        std::string received_message = std::string((char*)chat_memory);
        if (received_message != "") {
            std::cout << "Received: " << received_message << std::endl;
            // Очищаем общую память после прочтения сообщения
            sprintf((char*)chat_memory, "");
        }

        sem_post(semaphore); // Освобождаем семафор

        sleep(1); // Пауза перед следующей проверкой сообщений
    }

    munmap(chat_memory, 4096);
    close(shared_memory_fd);
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME);

    return 0;
}