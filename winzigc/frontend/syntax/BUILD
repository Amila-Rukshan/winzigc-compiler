load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "kind",
    hdrs = ["kind.h"],
)

cc_library(
    name = "token",
    hdrs = ["token.h"],
    deps = [":kind"],
    visibility = ["//winzigc/frontend/lexer:__pkg__"],
)
