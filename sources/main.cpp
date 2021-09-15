/*
** EPITECH PROJECT, 2020
** main
** File description:
** Entry of the program
*/

#include <list>
#include <iostream>
#include <thread>
#include "macros.hpp"
#include "thread/Pool.hpp"
#include "thread/TimedFunction.hpp"

void displayI(uint_fast16_t num [[ gnu::unused]])
{
    static std::atomic<size_t> i [[ gnu::unused]] = 1;

    for (;;) {
#if DEBUG > 0
    DEBUG_MSG("display: " << i++ << " (" << num << ")");
#else
    std::cout << "display: " << i++ << " (" << num << ")" << std::endl;
#endif
    }
}

int main()
{
    DEBUG_TIME;

    ThreadPool tp1;
    TimedFunction tf1(tp1, std::bind(displayI, 1), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf2(tp1, std::bind(displayI, 1), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf3(tp1, std::bind(displayI, 1), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf4(tp1, std::bind(displayI, 1), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf5(tp1, std::bind(displayI, 1), std::chrono::seconds(1), std::chrono::seconds(0), 5);

    ThreadPool tp2;
    TimedFunction tf6(tp2, std::bind(displayI, 2), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf7(tp2, std::bind(displayI, 2), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf8(tp2, std::bind(displayI, 2), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf9(tp2, std::bind(displayI, 2), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf10(tp2, std::bind(displayI, 2), std::chrono::seconds(1), std::chrono::seconds(0), 5);

    ThreadPool tp3;
    TimedFunction tf11(tp3, std::bind(displayI, 3), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf12(tp3, std::bind(displayI, 3), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf13(tp3, std::bind(displayI, 3), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf14(tp3, std::bind(displayI, 3), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf15(tp3, std::bind(displayI, 3), std::chrono::seconds(1), std::chrono::seconds(0), 5);

    ThreadPool tp4;
    TimedFunction tf16(tp4, std::bind(displayI, 4), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf17(tp4, std::bind(displayI, 4), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf18(tp4, std::bind(displayI, 4), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf19(tp4, std::bind(displayI, 4), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf20(tp4, std::bind(displayI, 4), std::chrono::seconds(1), std::chrono::seconds(0), 5);

    ThreadPool tp5;
    TimedFunction tf21(tp5, std::bind(displayI, 5), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf22(tp5, std::bind(displayI, 5), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf23(tp5, std::bind(displayI, 5), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf24(tp5, std::bind(displayI, 5), std::chrono::seconds(1), std::chrono::seconds(0), 5);
    TimedFunction tf25(tp5, std::bind(displayI, 5), std::chrono::seconds(1), std::chrono::seconds(0), 5);

    tp1.join();
    tp2.join();
    tp3.join();
    tp4.join();
    tp5.join();

    return 0;
}
