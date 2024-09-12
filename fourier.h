#pragma once

#include <cmath>
#include <complex>
#include <vector>

extern const double PI;

void DFT(std::vector<double>& signal, std::vector<std::complex<double>>& spectrum, int N);
void IDFT(std::vector<std::complex<double>>& spectrum, std::vector<double>& signal, int N);
void DFT_float(std::vector<float>& signal, std::vector<std::complex<float>>& spectrum, int N);
void IDFT_float(std::vector<std::complex<float>>& spectrum, std::vector<float>& signal, int N);