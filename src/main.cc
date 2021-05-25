//
// Created by huhua on 2021/5/21.
//

#include <iostream>
#include "./cpu_simulator.h"

int main() {
    CpuSimulator simulator;
    simulator.Construct(680, 480, 2, 2);
    simulator.Start();
    return 0;
}
