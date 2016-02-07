context("Smoothing option")

test_that("Smoothing", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 11, 13.4, 16, 18.8)
  Accuracy = 0.11

  #result: [-0.3854547619047608, 10.556883333333333, 21.576450000000001] [0.0, 0.69999999999999996, 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy, check.strict.adm = F)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(-0.3854547, 10.5568833, 21.57645), tolerance = 1e-06)
  expect_equal(CompressedSample$Y, c(0, 0.7, 1), tolerance = 1e-06)
})
