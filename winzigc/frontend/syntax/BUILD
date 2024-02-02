load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "kind_lib",
    hdrs = ["kind.h"],
)

cc_library(
    name = "token_lib",
    hdrs = ["token.h"],
    deps = [":kind_lib"],
    visibility = [
        "//winzigc/frontend/lexer:__pkg__",
        "//winzigc/frontend/parser:__pkg__",
        "//test/frontend/lexer:__pkg__",
    ],
)
