#include "PolynomialLinkedList.h"
#include <stdio.h>
#include <math.h>

/* TermElement Implementation */
TermElement::TermElement() {
  degree = 0;
  coefficient = 0.0;
  next = NULL;
}
TermElement::TermElement(int degree, double coefficient) {
  this->degree = degree;
  this->coefficient = coefficient;
  next = NULL;
}
double TermElement::evaluate(double x) {                   /* 1. evaluate */
  if (degree != 0) {
    return coefficient * pow(x, degree);
  } else {
    return coefficient;
  }
}
void TermElement::print(bool first) {
  if (first) {
    printf("%g",coefficient);
  } else {
    if (coefficient < 0.0) {
      printf(" - %g",-coefficient);
    } else {
      printf(" + %g",coefficient);
    }
  }
  if (degree == 1) {
    printf("x");
  } else if (degree != 0) {
    printf("x^%d",degree);
  }
}

/* SparsePoly Implementation */
SparsePoly::SparsePoly() {
  root = NULL;
}
SparsePoly::~SparsePoly() {                                /* 5. freeList */
  TermElement *term = root;
  while (term) {
    TermElement *it = term;
    term = term->next;
    delete it;
  }
}
void SparsePoly::addTerm(int degree, double coefficient) { /* 2. addTerm */
  bool added = false;
  TermElement **curr = &root;
  while (*curr && (!added)) { /* for (TermElement **curr = &root; *curr && (!added);) except curr may be needed later */
    TermElement *term = *curr; /* If this loop is confusing, see http://wordaligned.org/articles/two-star-programming */
    if (degree > term->degree) {
      *curr = new TermElement(degree, coefficient);
      (*curr)->next = term;
      added = true;
    } else if (degree == term->degree) {
      term->coefficient += coefficient;
      if (fabs(term->coefficient) < 0.000001) { /* close enough */
        *curr = term->next;
        delete term;
      }
      added = true;
    } else {
      curr = &term->next;
    }
  }
  if (!added) {
    *curr = new TermElement(degree, coefficient);
  }
}
double SparsePoly::evaluate(double x) {                    /* 3. evaluatePolynomial */
  double result = 0.0;
  for (TermElement *term = root; term; term = term->next) {
    result += term->evaluate(x);
  }
  return result;
}
SparsePoly *SparsePoly::derivative() {                     /* 4. derivative */
  SparsePoly *result = new SparsePoly;
  for (TermElement *term = root; term; term = term->next) {
    if (term->degree != 0) {
      result->addTerm(term->degree - 1, term->coefficient * term->degree);
    }
  }
  return result;
}
void SparsePoly::print() {                                 /* 6. printPolynomial */
  if (root == NULL) {
    printf("Empty Polynomial\n");
    return;
  }
  root->print(true);
  for (TermElement *term = root->next; term; term = term->next) {
    term->print();
  }
  printf("\n");
}
void SparsePoly::add(SparsePoly *other) {
  if (other == NULL) {
    return;
  }
  for (TermElement *term = other->root; term; term = term->next) {
    addTerm(term->degree, term->coefficient);
  }
}
#ifdef USE_ASSIGNMENT_INTERFACE
/* These should not exist, workaround for section below */
SparsePoly::SparsePoly(TermElement *root) {
  this->root = root;
}
TermElement *SparsePoly::disassociate() {
  TermElement *r = root;
  root = NULL;
  return r;
}

/** 
 * Function intermediaries to implement assignment interface
 * These functions should be avoided where possible
 */
double evaluate(TermElement *e, double x) {
  return e->evaluate(x);
}
TermElement *addTerm(TermElement *root, int degree, double coefficient) {
  SparsePoly poly(root);
  poly.addTerm(degree, coefficient);
  return poly.disassociate();
}
double evaluatePolynomial(TermElement *root, double x) {
  SparsePoly poly(root);
  double r = poly.evaluate(x);
  poly.disassociate();
  return r;
}
TermElement *derivative(TermElement *root) {
  SparsePoly poly(root);
  TermElement *r = poly.derivative()->disassociate();
  poly.disassociate();
  return r;
}
void freeList(TermElement *root) {
  SparsePoly *poly = new SparsePoly(root);
  delete poly;
}
#endif

#ifdef USE_SPARSEPOLY_MAIN
int main(int argc, char **argv) {
  SparsePoly *test = new SparsePoly;
  test->print();
  test->addTerm(5,5.0);
  test->print();
  test->addTerm(5,-5.0);
  test->print();
  test->addTerm(2,-2.0);
  test->addTerm(0,4.0);
  test->addTerm(1,3.5);
  test->print();
  printf("The result of evaluating for x=1 is %g\n", test->evaluate(1.0));
  SparsePoly *deriv = test->derivative();
  deriv->print();
  printf("The result of evaluating for x=1 is %g\n", deriv->evaluate(1.0));
  test->add(deriv);
  test->print();
  delete test;
  delete deriv;
  
  return 0;
}
#endif
