#include "Calculator.hpp"

#include "CalculatorUI/CalculatorUI.hpp"
#include "CalculatorWorker/CalculatorWorker.hpp"

void runCalculator() {
    CalculatorWorker worker;
    worker.setExpression("x");

    CalculatorUI ui(worker);
    ui.runUI();
}