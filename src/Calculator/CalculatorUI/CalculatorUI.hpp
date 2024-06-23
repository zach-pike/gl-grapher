#pragma once

#include "Utility/Logger/Logger.hpp"

class CalculatorWorker;

class CalculatorUI {
private:
    Logger logger;
    CalculatorWorker& worker;

public:
    CalculatorUI(CalculatorWorker& worker);
    ~CalculatorUI();

    void runUI();
};