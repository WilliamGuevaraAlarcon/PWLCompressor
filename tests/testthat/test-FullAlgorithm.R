context("Example 6.13 as in paper")

test_that("FullAlgorithm", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 13, 13.4, 16, 18.8)
  Accuracy = 0.08

  #result: [0.2333333  4.3666667  7.4333333 11.5500000 19.0500000] [0.0 0.3 0.6 0.6 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy, enforced.quantiles = 0.3)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(0.233, 4.367, 7.433, 11.55, 19.05), tolerance = 1e-03)
  expect_equal(CompressedSample$Y, c(0.0, 0.3, 0.6, 0.6, 1.0), tolerance = 1e-06)
})



