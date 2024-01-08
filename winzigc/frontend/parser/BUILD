load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "parser_lib",
    hdrs = [
        "parser.h",
        "parser.cc",
    ],
    deps = [
        "//winzigc/frontend/syntax:token_lib",
        "//winzigc/frontend/ast:ast_lib",
    ],
    visibility = ["//winzigc/main:__pkg__"],
)
