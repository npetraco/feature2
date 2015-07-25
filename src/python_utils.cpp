#include <Rcpp.h>
#include <Python.h>
#include <stdlib.h>
//#include <dlfcn.h>

using namespace Rcpp;

//Run Python commands from R
//[[Rcpp::export]]
void pyrun(std::string command) {
  PyRun_SimpleString(command.c_str());
}

//You need to call this first
//[[Rcpp::export]]
void initialize_python() {
  Py_Initialize();
  pyrun("import sys");
  pyrun("sys.path.append('/usr/local/lib/python2.7/site-packages')");
  //pyrun("import cv2");
}

//Call after you're done
//[[Rcpp::export]]
void finalize_python() {
    Py_Finalize();
}