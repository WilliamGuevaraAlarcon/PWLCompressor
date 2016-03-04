context("Check the compressor result")

test_that("SimpleExemple", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 13, 13.4, 16, 18.8)
  Accuracy = 0.01

  CompressedSample = PWLCompressor(Sample, Accuracy)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(0.85, 1.75, 3.8656, 5.0344, 8.0656, 12.9, 13.5, 16, 16, 18.8, 18.8), tolerance = 1e-03)
  expect_equal(CompressedSample$Y, c(0, 0.2, 0.2, 0.4, 0.6, 0.6, 0.8, 0.8, 0.9, 0.9, 1), tolerance = 1e-06)
})



