context("This test contains segments which are not at the beginning or end, and which violate the strict admissibility condition")

test_that("test-StrictAdmissibility", {
  Sample = c(1, 1.6, 4.3, 4.6, 6, 7.1, 11, 13.4, 16, 18.8)
  Accuracy = 0.12

  #result: [0.4333333  7.7666667 11.0000000 11.0000000 18.4000000 18.8000000 18.8000000] [0.0 0.6 0.6 0.7 0.9 0.9 1.0]
  #      : [ -0.4285048 10.5999333 21.5334000] [0.0 0.7 1.0]
  CompressedSampleStrict    = PWLCompressor(Sample, Accuracy, check.strict.adm = TRUE)
  CompressedSampleNonStrict = PWLCompressor(Sample, Accuracy, check.strict.adm = FALSE)
  print(CompressedSampleStrict)
  print(CompressedSampleNonStrict)

  expect_equal(CompressedSampleStrict$X, c(0.433, 7.767, 11.0, 11.0, 18.4, 18.8, 18.8), tolerance = 1e-03)
  expect_equal(CompressedSampleStrict$Y, c(0.0, 0.6, 0.6, 0.7, 0.9, 0.9, 1.0), tolerance = 1e-06)
  expect_equal(CompressedSampleNonStrict$X, c(-0.429, 10.6, 21.533), tolerance = 1e-03)
  expect_equal(CompressedSampleNonStrict$Y, c(0.0, 0.7, 1.0), tolerance = 1e-06)
})
