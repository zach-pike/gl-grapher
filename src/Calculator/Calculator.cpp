#include "Calculator.hpp"

#include "CalculatorUI/CalculatorUI.hpp"
#include "CalculatorWorker/CalculatorWorker.hpp"

void runCalculator() {
    CalculatorWorker worker;
    CalculatorUI ui(worker);
    worker.setViewportInfo(CalculatorWorker::CalculatorViewportPosition{ -10, -10, 20, 20 });

    ui.runUI();
}