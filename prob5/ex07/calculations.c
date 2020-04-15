#include "calculations.h"

double sum(double a, double b) {
  return a + b;
}

double division(double a, double b) {
  if (b != 0)
    return a / b;
  return INT_MAX;
}

double multiplication(double a, double b) {
  return a * b;
}

double subtraction(double a, double b) {
  return a - b;
}
