context("Negative Incremental Removal option with enforced quantiles")

test_that("NegativeIncrementRemovalWithEnforcedInterpolationQuantile", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 13, 13.4, 16, 18.8)
  Accuracy = 0.25

  #result: [0.9326531  3.6673469 18.8755102] [0.0, 0.3, 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy, enforced.quantiles = 0.3)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(0.933, 3.667, 18.875), tolerance = 1e-03)
  expect_equal(CompressedSample$Y, c(0.0, 0.3, 1.0), tolerance = 1e-06)
})

