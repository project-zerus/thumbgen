cc_library(
  name = 'common',
  deps = [
    '//external:boost',
    '//external:gflags',
    '//external:glog',
  ],
  hdrs = [
    'CommonUtils.h',
  ],
  srcs = [
    'CommonUtils.cpp',
  ]
)

cc_binary(
  name = 'thumbgen',
  srcs = [
    'ThumbGen.cpp',
  ],
  deps = [
    ':common',
  ]
)

cc_test(
  name = 'common_test',
  deps = [
    ':common',
    '//external:gtest_main',
  ],
  srcs = [
    'CommonUtilsTest.cpp'
  ],
)
