#include "CalculatorWorker.hpp"

#include <functional>
#include <iostream>

static double linearInterp(double x, double inMin, double inMax, double outMin, double outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void CalculatorWorker::worker() {
    while(workerThreadRunning) {
        if (calculationDone == true) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5ms);
            continue;
        }

        // Data not ready
        std::scoped_lock lock(currentExpressionLock, viewportInfoLock, dataLock);

        data.clear();

        // Evaluate over x
        for (xVar = viewportInfo.x; xVar < (viewportInfo.x+viewportInfo.width); xVar += .01f) {
            double yVal = currentExpression.value();

            float scaledX = linearInterp(xVar, viewportInfo.x, viewportInfo.x + viewportInfo.width, -1, 1);
            float scaledY = linearInterp(yVal, viewportInfo.y, viewportInfo.y + viewportInfo.height, -1, 1);

            data.push_back(glm::vec2(scaledX, scaledY));
        }

        calculationDone = true;
        newDataAvailable = true;
    }
}

CalculatorWorker::CalculatorWorker() {
    symbolTable.add_variable("x", xVar);
    symbolTable.add_constants(); // Adds pi, epsilion, and infinity
    currentExpression.register_symbol_table(symbolTable);

    workerThreadRunning = true;
    workerThread = std::thread(std::bind(&CalculatorWorker::worker, this));
}

CalculatorWorker::~CalculatorWorker() {
    workerThreadRunning = false;
    workerThread.join();
}

void CalculatorWorker::setExpression(std::string expression) {
    std::scoped_lock lock(currentExpressionLock);

    exprtk::parser<double> parser;                    
    parser.compile(expression, currentExpression);

    // Tell thread that calculation needs to be done
    calculationDone = false;
}

void CalculatorWorker::setViewportInfo(CalculatorViewportPosition pos) {
    std::scoped_lock lock(viewportInfoLock);

    viewportInfo = pos;
    calculationDone = false;
}

bool CalculatorWorker::isNewDataAvailable() {
    return newDataAvailable;
}

std::vector<glm::vec2> CalculatorWorker::getData() {
    std::scoped_lock lock(dataLock);

    newDataAvailable = false;
    return data;
}