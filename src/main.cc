//
// Created by huhua on 2021/5/21.
//

#include <iostream>
#define OLC_PGE_APPLICATION
#include "./cpu_simulator.h"

int main() {
    CpuSimulator simulator;
    simulator.Construct(680, 480, 2, 2);
    simulator.Start();
//    std::cin.get();
    return 0;
}
