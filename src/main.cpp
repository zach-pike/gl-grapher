#include <iostream>
#include <vector>

#include "Calculator/CalculatorUI/CalculatorUI.hpp"
#include "Calculator/CalculatorWorker/CalculatorWorker.hpp"

int main() {
    CalculatorWorker worker;
    CalculatorUI calc(worker);

    calc.runUI();

    return 0;
}