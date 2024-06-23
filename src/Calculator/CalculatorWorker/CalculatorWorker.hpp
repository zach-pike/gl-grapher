#pragma once

#include "exprtk/exprtk.hpp"

#include <glm/glm.hpp>

#include <string>
#include <mutex>
#include <thread>


class CalculatorWorker {
public:
    struct CalculatorViewportPosition {
        double x, y, width, height;
    };
private:
    double xVar;
    exprtk::symbol_table<double> symbolTable;

    exprtk::expression<double>   currentExpression;
    std::mutex                   currentExpressionLock;

    bool        workerThreadRunning = false;
    std::thread workerThread;

    std::atomic_bool       calculationDone = false;
    std::atomic_bool       newDataAvailable = false;
    std::vector<glm::vec2> data;
    std::mutex             dataLock;

    CalculatorViewportPosition viewportInfo;
    std::mutex                 viewportInfoLock;

    void worker();
public:
    CalculatorWorker();
    ~CalculatorWorker();

    void setExpression(std::string expression);

    void setViewportInfo(CalculatorViewportPosition pos);

    bool isNewDataAvailable();

    std::vector<glm::vec2> getData();
};