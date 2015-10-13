#ifndef RCPPEXPORTS_H
#define RCPPEXPORTS_H

#include <vector>
#include <Rcpp.h>
#include "double_vec.h"
#include "compressor.h"

RcppExport SEXP Compressor(SEXP sample_, SEXP eps_);

#endif
