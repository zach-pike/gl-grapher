#include "Calculator.hpp"

#include "CalculatorUI/CalculatorUI.hpp"
#include "CalculatorWorker/CalculatorWorker.hpp"

void runCalculator() {
    CalculatorWorker worker;
    CalculatorUI ui(worker);
    worker.setViewportInfo(CalculatorWorker::CalculatorViewportPosition{ -1, -1, 2, 2 });

    ui.runUI();
}