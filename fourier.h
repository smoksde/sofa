#pragma once

#include <cmath>
#include <complex>
#include <vector>

extern const double PI;

void DFT(std::vector<double>& signal, std::vector<std::complex<double>>& spectrum, int N);
void IDFT(std::vector<std::complex<double>>& spectrum, std::vector<double>& signal, int N);