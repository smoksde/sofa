#pragma once

#include <cmath>
#include <complex>
#include <vector>

extern const double PI;

void DFT(std::vector<double>& signal, std::complex<double>* spectrum, int N);
void IDFT(std::complex<double>*, std::vector<double>& signal, int N);