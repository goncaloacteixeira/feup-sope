#include "calculations.h"

float sum(double a, double b) {
  return a + b;
}

float division(double a, double b) {
  if (b != 0)
    return a / b;
  return INT_MAX;
}

float multiplication(double a, double b) {
  return a * b;
}

float subtraction(double a, double b) {
  return a - b;
}
