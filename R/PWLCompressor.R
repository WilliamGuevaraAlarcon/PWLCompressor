# Some useful keyboard shortcuts for package authoring:
#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

#' Compress an edf to a pwl distribution
#'
#' @param sample A vector with the sample values
#' @param accuracy Relative accuracy of the approximation
#' @return A list where X and Y cointains the abscissas and ordinates of the points for the admissible pwl approximation
#' @references Arbenz, W. and Guevara W. (2015) Risk Measure Preserving Approximation of Univariate Monte Carlo Simulations Results with Insurance Applications \url{https://sites.google.com/site/philipparbenz/home/pwl-approximation}
#' @examples
#' ##Minimal example
#' Sample <- c(1,1.6,4.3,4.6,6,7.1,13,13.4,16,18.8)
#' PWLapprox <- PWLCompressor(Sample, 0.01)

PWLCompressor <- function(sample, accuracy = 0.01){
  .Call('Compressor', sample, accuracy)
}

