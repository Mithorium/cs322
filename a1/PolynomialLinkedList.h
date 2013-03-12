#ifndef SPARSEPOLY_H
#define SPARSEPOLY_H

#define USE_ASSIGNMENT_INTERFACE

class TermElement {
  public:
  int degree;
  double coefficient;
  TermElement *next;
  
  TermElement();
  TermElement(int degree, double coefficient);
  double evaluate(double x);                    /* 1. evaluate */
  void print(bool first = false);
};
class SparsePoly {
  private:
  TermElement *root;
  public:
  SparsePoly();
  ~SparsePoly();                                /* 5. freeList */
  void addTerm(int degree, double coefficient); /* 2. addTerm */
  double evaluate(double x);                    /* 3. evaluatePolynomial */
  SparsePoly *derivative();                     /* 4. derivative */
  void print();                                 /* 6. printPolynomial */
  void add(SparsePoly *other);
#ifdef USE_ASSIGNMENT_INTERFACE
  /* These should not exist, workaround for assignment */
  SparsePoly(TermElement *root);
  TermElement *disassociate();
#endif
};

#ifdef USE_ASSIGNMENT_INTERFACE
/** 
 * Function intermediaries to implement assignment interface
 * These functions should be avoided where possible
 */
double evaluate(TermElement *e, double x);
TermElement *addTerm(TermElement *root, int degree, double coefficient);
double evaluatePolynomial(TermElement *root, double x);
TermElement *derivative(TermElement *root);
void freeList(TermElement *root);
#endif

#endif /* SPARSEPOLY_H */
