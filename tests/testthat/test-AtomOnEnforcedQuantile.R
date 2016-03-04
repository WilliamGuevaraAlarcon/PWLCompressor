context("Ensure that an enforced quantile on an atom is removed from the solution")

test_that("test-AtomOnEnforcedQuantile", {
  Sample = c(1, 2, 4, 5 , 5, 5, 5, 7, 10, 11)
  Accuracy = 0.1

  #result: [0.2333333  4.3666667  7.4333333 11.5500000 19.0500000] [0.0 0.3 0.6 0.6 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy, enforced.quantiles = 0.5, min.size.atom.detection = 10, rel.atom.detection = 0.1)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(-0.333, 5.0, 5.0, 6.666, 12), tolerance = 1e-03)
  expect_equal(CompressedSample$Y, c(0.0, 0.3, 0.7, 0.7, 1.0), tolerance = 1e-06)
})
