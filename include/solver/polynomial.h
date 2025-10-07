#ifndef __VISIONLIB_SOLVER_POLYNOMIAL_H__
#define __VISIONLIB_SOLVER_POLYNOMIAL_H__

namespace rm {


double getPrediction(double x, double y);

void initFit(int x_max, int y_max);

void calculateFactors(std::vector<double>& inputs, bool isNeedR_2);

void getFitFactors();

double getR_2();

};

#endif