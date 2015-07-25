#include <Rcpp.h>
#include <Python.h>
#include <stdlib.h>
using namespace Rcpp;

//Convert NumericVector to Python List
PyObject* numvec_to_list(NumericVector x) {
  int n = x.length();
  PyObject *xpy = PyList_New(n); //Make new list
  PyObject *f;
  
  for (int i=0; i<n; i++) {
    f = PyFloat_FromDouble(x[i]);
    PyList_SetItem(xpy, i, f); //Fill list from NumericVector
  }   
  return(xpy);
}

//Copy a numeric vector from R to Python
//[[Rcpp::export]]
void numvec_to_python(std::string name, NumericVector x) {
  PyObject *xpy = numvec_to_list(x);
  PyObject *m = PyImport_AddModule("__main__");
  PyObject *main = PyModule_GetDict(m); //Get the locals dictionary of __main__ module
  PyDict_SetItemString(main, name.c_str(), xpy); //Add variable to that dictionary
}