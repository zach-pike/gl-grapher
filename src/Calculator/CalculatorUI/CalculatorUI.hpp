#pragma once

#include "Utility/Logger/Logger.hpp"

#include "Calculator/CalculatorWorker/CalculatorWorker.hpp"

class CalculatorUI {
private:
    Logger logger;
    CalculatorWorker& worker;
    CalculatorWorker::CalculatorViewportPosition viewportInfo;

public:
    CalculatorUI(CalculatorWorker& worker);
    ~CalculatorUI();

    void runUI();
};