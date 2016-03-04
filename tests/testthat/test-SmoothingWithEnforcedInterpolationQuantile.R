context("Smoothing option with enforced quantiles")

test_that("SmoothingWithEnforcedInterpolationQuantile", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 13, 13.4, 16, 18.8)
  Accuracy = 0.25

  #result: [-0.7303333  8.9303333 21.6696667] [0.0, 0.6, 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy, enforced.quantiles = 0.6)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(-0.730, 8.930, 21.670), tolerance = 1e-03)
  expect_equal(CompressedSample$Y, c(0, 0.6, 1), tolerance = 1e-06)
})

