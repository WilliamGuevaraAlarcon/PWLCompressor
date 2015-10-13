#include "Rcpp_exports.h"
#include <Rcpp.h>

using namespace Rcpp;

RcppExport SEXP Compressor(SEXP sample_, SEXP eps_){
BEGIN_RCPP
	DoubleVec sample = Rcpp::as<DoubleVec>(sample_);
	double eps = Rcpp::as<double>(eps_);

	PWLCompressor Comp = PWLCompressor(sample, eps);

	std::vector<double> Xval = Comp.GetResult().Xvalues;
	std::vector<double> Yval = Comp.GetResult().Yvalues;

	Rcpp::List ret;
	ret ["X"] = Xval;
	ret ["Y"] = Yval;

	return (ret);
END_RCPP
}
