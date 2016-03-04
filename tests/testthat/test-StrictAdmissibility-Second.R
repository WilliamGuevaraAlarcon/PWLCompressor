context("Test strict admissibility condition")

test_that("test-StrictAdmissibility-Second", {
  Sample = c( 5.345,  7.343,   11.6, 5.539, 5.199,  7.402, 5.021,  5.212, 5.074,  8.391,
              5.642,  6.223,  6.241, 7.363, 14.392, 6.732, 5.292,  9.049,  8.82, 10.113,
             10.712, 28.547, 10.295, 9.305, 15.707, 5.966, 9.698, 18.212, 6.189, 12.805)
  Accuracy = 0.34

  #result: [3.764188 10.582979 12.001667 13.863000 27.781000] [0.0 0.8 0.8 0.9 1.0]
  #      : [3.543514 12.083375 29.560625] [0.0 0.9 1.0]
  CompressedSampleStrict    = PWLCompressor(Sample, Accuracy, check.strict.adm = TRUE)
  CompressedSampleNonStrict = PWLCompressor(Sample, Accuracy, check.strict.adm = FALSE)
  print(CompressedSampleStrict)
  print(CompressedSampleNonStrict)

  expect_equal(CompressedSampleStrict$X, c(3.764, 10.583, 12.002, 13.863, 27.781), tolerance = 1e-03)
  expect_equal(CompressedSampleStrict$Y, c(0.0, 0.8, 0.8, 0.9, 1.0), tolerance = 1e-06)
  expect_equal(CompressedSampleNonStrict$X, c(3.544, 12.084, 29.561), tolerance = 1e-03)
  expect_equal(CompressedSampleNonStrict$Y, c(0.0, 0.9, 1.0), tolerance = 1e-06)
})
