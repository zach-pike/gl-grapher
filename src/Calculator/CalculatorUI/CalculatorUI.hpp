#pragma once

#include "Utility/Logger/Logger.hpp"


class CalculatorUI {
private:
    Logger logger;

public:
    CalculatorUI();
    ~CalculatorUI();

    void runUI();
};