#ifndef __visionlib_SOLVER_POLYNOMIAL_H__
#define __visionlib_SOLVER_POLYNOMIAL_H__

#include <vector>

namespace rm {


double getPrediction(double x, double y);

void initFit(int x_max, int y_max);

void calculateFactors(std::vector<double>& inputs, bool isNeedR_2);

void getFitFactors();

double getR_2();

};

#endif