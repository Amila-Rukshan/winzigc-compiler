load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "parser_lib",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = [
        "//test/frontend/parser:__pkg__",
        "//test/visitor/semantic:__pkg__",
        "//winzigc/main:__pkg__",
    ],
    deps = [
        "//winzigc/frontend/ast:ast_lib",
        "//winzigc/frontend/syntax:token_lib",
        "@com_github_google_glog//:glog",
    ],
)
