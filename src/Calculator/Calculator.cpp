#include "Calculator.hpp"

#include "CalculatorUI/CalculatorUI.hpp"
#include "CalculatorWorker/CalculatorWorker.hpp"

void runCalculator() {
    CalculatorWorker worker;
    CalculatorUI ui(worker);
}