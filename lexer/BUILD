load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "lexer",
    srcs = ["lexer.cc"],
    hdrs = ["lexer.h"],
    deps = [
        "@com_github_google_glog//:glog",
        "//syntax:token"
    ],
    visibility = ["//main:__pkg__"],
)
