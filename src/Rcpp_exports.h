#ifndef RCPPEXPORTS_H
#define RCPPEXPORTS_H

#include <vector>
#include <Rcpp.h>
#include "double_vec.h"
#include "compressor.h"

RcppExport SEXP Compressor(SEXP sample_, SEXP eps_, SEXP quantile_, SEXP minatom_, SEXP relatom_, SEXP fullcheck_, SEXP smooth_);

#endif
