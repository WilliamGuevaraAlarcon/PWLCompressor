#include "Rcpp_exports.h"
#include <Rcpp.h>

using namespace Rcpp;

RcppExport SEXP Compressor(SEXP sample_, SEXP eps_, SEXP quantile_, SEXP minatom_, SEXP relatom_, SEXP fullcheck_, SEXP smooth_){
BEGIN_RCPP
	DoubleVec sample = Rcpp::as<DoubleVec>(sample_);
	double eps = Rcpp::as<double>(eps_);
	DoubleVec quantile = Rcpp::as<DoubleVec>(quantile_);
  int minatom = Rcpp::as<int>(minatom_);
  double relatom = Rcpp::as<double>(relatom_);
  bool fullcheck = Rcpp::as<bool>(fullcheck_);
  bool smooth = Rcpp::as<bool>(smooth_);

	PWLCompressor Comp = PWLCompressor(sample, eps, quantile, minatom, relatom, fullcheck, smooth);

	std::vector<double> Xval = Comp.GetResult().Xvalues;
	std::vector<double> Yval = Comp.GetResult().Yvalues;

	Rcpp::List ret;
	ret ["X"] = Xval;
	ret ["Y"] = Yval;

	return (ret);
END_RCPP
}
