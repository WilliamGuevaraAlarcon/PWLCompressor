# Some useful keyboard shortcuts for package authoring:
#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

#' Compress an edf to a pwl distribution
#'
#' @param sample A vector with the sample values
#' @param accuracy Relative accuracy of the approximation
#' @param quantile A vector with the quantiles which shall be contained in the basis of the approximation                         approximation.
#' @param minatom Minimum sample size which needs to be satisfied such that the algorithm looks for atoms
#' @param relatom Relative threshold for atoms to be detected
#' @param fullcheck A boolean to determine whether the algorithm should check whether that an admissible approximation is also strictly admissible
#' @return A list where X and Y cointains the abscissas and ordinates of the points for the admissible pwl approximation
#' @examples
#' ##Minimal example
#' Sample <- c(1,1.6,4.3,4.6,6,7.1,13,13.4,16,18.8)
#' PWLapprox <- PWLCompressor(Sample, 0.01)

PWLCompressor <- function(sample, accuracy = 0.01, quantile = c(0,1), minatom = 1000, relatom = 0.005, fullcheck = F){
  .Call('Compressor', sample, accuracy, quantile, minatom, relatom, fullcheck)
}

