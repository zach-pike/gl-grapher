#pragma once

#include "CalculatorUI/CalculatorUI.hpp"
#include "CalculatorWorker/CalculatorWorker.hpp"

class Calculator {
private:
    CalculatorUI ui;
    CalculatorWorker worker;

public:
    Calculator();
    ~Calculator();
};