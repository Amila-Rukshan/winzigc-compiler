load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pure_lib",
    hdrs = [
        "pure.h",
    ],
    visibility = [
        "//winzigc/frontend/ast:__pkg__",
        "//winzigc/visitor/codegen:__pkg__",
        "//winzigc/visitor/semantic:__pkg__",
    ],
)
