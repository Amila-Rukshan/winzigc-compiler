load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "lexer_lib",
    srcs = ["lexer.cc"],
    hdrs = ["lexer.h"],
    visibility = [
        "//test/frontend/lexer:__pkg__",
        "//test/visitor/semantic:__pkg__",
        "//winzigc/main:__pkg__",
    ],
    deps = [
        "//winzigc/frontend/syntax:token_lib",
        "@com_github_google_glog//:glog",
        "@llvm-project//llvm:Core",
        "@llvm-project//llvm:Support",
    ],
)
