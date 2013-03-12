#include <math.h>
#include <iostream>
#include <time.h>
#include "PolynomialLinkedList.h"

//Take this code and copy/paste it into your code for comparing the amount of time between the two approaches
//Make sure to remember to include the libraries in your source file as well.

using namespace std;

double evaluateArrayPolynomial(const double coefficients[], int maximumDegree, double x)
{
  double sum = 0;
  for (int i = 0; i < maximumDegree; i++)
  {
    sum = sum + pow(x, i) * coefficients[i];
  }

  return sum;
}

//This function calls evaluatePolynomial (written by you) and evaluteArrayPolynomial
//many times each on a sparse polynomial. It then prints the amount of time spent on each 
//computation.
void compareTimes() 
{
  //CHANGE THE FOLLOWING COUNTER TO INCREASE THE NUMBER OF STEPS OF EACH
  const int numComputations = 100000;
  //CHANGE THE FOLLOWING CONSTANT TO SET THE MAXIMUM DEGREE
  //THIS CODE WILL STORE THE POLYNOMIAL x^N-1 where n takes the value set in the next step
  const int N = 400;
  
  //construct a sparse polynomial using the linked list representation
  SparsePoly *poly = new SparsePoly;
  poly->addTerm(1,-1);
  poly->addTerm(N,1);

  //construct a sparse polynomial using the naive array representation
  double coefficients[N+1];
  for (int i = 0; i < N+1; i++) {
    coefficients[i] = 0;
  }

  coefficients[N] = 1;
  coefficients[0] = -1;


  double duration;
  //clock() is a function that gets the number of ticks since the program started
  //we store this amount at the beginning and then will calcuate the difference at each step
  clock_t start = clock();
  //call evaluatePolynomial many times
  for (int j = 0; j < numComputations; j++) 
  {
    poly->evaluate(2);
  }

  //store the amount of ticks since we last measured
  double linkedListTime = clock() - start;

  //now call the array evaluate polynomial many times
  for (int j = 0; j < numComputations; j++)
  {
    evaluateArrayPolynomial(coefficients, 401, 2) ;
  }
  
  //store the amount of ticks since we last measured
  double arrayTime = clock() - linkedListTime;
  cout << "Amount of time for first algorithm is "<< linkedListTime << " and array time is " << arrayTime << endl;
  cout << clock();
}

int main() {
  compareTimes();
}
