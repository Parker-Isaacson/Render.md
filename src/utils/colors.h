#ifndef COLORS_H
#define COLORS_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

int hexToInt(const std::string& hex);
int clamp(int value);
std::string darkenColor(const std::string& hexColor, double percent);

#endif // COLORS_H