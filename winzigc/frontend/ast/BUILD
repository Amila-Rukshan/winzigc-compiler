load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "ast_lib",
    hdrs = [
        "expr.h",
        "type.h",
        "user_type.h",
        "var.h",
        "function.h",
        "program.h",
        "visitor.h",
        "location.h",
    ],
    srcs = [
        "expr.cc",
        "type.cc",
        "user_type.cc",
        "var.cc",
    ],
    deps = [
        "@llvm-project//llvm:Core",
        "@llvm-project//llvm:Support",
        "//winzigc/common:pure_lib",
    ],
    visibility = [
        "//winzigc/frontend/parser:__pkg__",
        "//winzigc/visitor:__pkg__",
    ],
)
