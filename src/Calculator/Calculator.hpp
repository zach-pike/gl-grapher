#pragma once

#include "Utility/Logger/Logger.hpp"


class Calculator {
private:
    Logger logger;

public:
    Calculator();
    ~Calculator();

    void runUI();
};