load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "parser_lib",
    hdrs = [ "parser.h"],
    srcs = ["parser.cc"],
    deps = [
        "@com_github_google_glog//:glog",
        "//winzigc/frontend/syntax:token_lib",
        "//winzigc/frontend/ast:ast_lib",
    ],
    visibility = [
        "//winzigc/main:__pkg__",
        "//test/frontend/parser:__pkg__",
    ],
)
