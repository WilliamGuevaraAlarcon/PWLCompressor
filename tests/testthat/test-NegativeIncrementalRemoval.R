context("Negative Incremental Removal option")

test_that("NegativeIncrementRemoval", {
  Sample = c(6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 10, 13, 15, 18, 21, 25)
  Accuracy = 0.03

  #result: [6.0, 6.0, 9.6452941176470581, 13.354705882352942, 26.145294117647058] [0.0, 0.6470588235294118, 0.6470588235294118, 0.76470588235294112, 1.0]
  CompressedSample = PWLCompressor(Sample, Accuracy)
  print(CompressedSample)

  expect_equal(CompressedSample$X, c(6.0, 6.0, 9.6452941, 13.3547058, 26.1452941), tolerance = 1e-06)
  expect_equal(CompressedSample$Y, c(0.0, 0.6470588, 0.6470588, 0.7647058, 1.0), tolerance = 1e-06)
})
