#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef struct {
	double sum;
	double div;
	double mul;
	double sub;
} response_t;


double sum(double a, double b);

double division(double a, double b);

double multiplication(double a, double b);

double subtraction(double a, double b);
