# Some useful keyboard shortcuts for package authoring:
#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

#' Compress an edf from a sample to a pwl distribution
#'
#' @useDynLib PWLCompressor
#' @importFrom Rcpp sourceCpp
#'
#' @param sample a vector with the sample values
#' @param accuracy relative accuracy of the approximation
#' @param enforced.quantiles an optional vector with the quantiles which shall be contained in the basis of the approximation. For these quantiles the PWL distribution will have the same TVaR as the empirical distribution
#' @param min.size.atom.detection minimum sample size which needs to be satisfied such that the algorithm looks for atoms
#' @param rel.atom.detection minimum proportion that a value has to be repeated in the sample in order to be considered as an atom
#' @param check.strict.adm logical. If TRUE the algorithm search for strict admissibility
#' @param smooth logical. If TRUE smoothing is applied
#' @references Arbenz, P. and Guevara-Alarcon. W. (2016), \emph{Risk Measure Preserving Piecewise Linear Approximation of Empirical Distributions.} Preprint. \url{http://sites.google.com/site/philipparbenz/home/pwl-approximation}
#' @return A list where X and Y cointains the abscissas and ordinates of the points for the admissible pwl approximation
#' @examples
#' ##  Minimal example
#' Sample <- c(1,1.6,4.3,4.6,6,7.1,13,13.4,16,18.8)
#' PWLapprox <- PWLCompressor(Sample, 0.01)
#' @export
PWLCompressor <- function(sample, accuracy = 0.01, enforced.quantiles = c(0,1), min.size.atom.detection = 1000, rel.atom.detection = 0.005, check.strict.adm = F, smooth = T){
  .Call('Compressor', sample, accuracy, enforced.quantiles, min.size.atom.detection, rel.atom.detection, check.strict.adm, smooth)
}

