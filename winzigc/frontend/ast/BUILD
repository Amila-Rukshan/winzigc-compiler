load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "ast_lib",
    srcs = [
        "expr.cc",
        "function.cc",
        "program.cc",
        "type.cc",
        "user_type.cc",
        "var.cc",
    ],
    hdrs = [
        "expr.h",
        "function.h",
        "location.h",
        "program.h",
        "type.h",
        "user_type.h",
        "var.h",
        "visitor.h",
    ],
    visibility = [
        "//winzigc/frontend/parser:__pkg__",
        "//winzigc/visitor/codegen:__pkg__",
        "//winzigc/visitor/semantic:__pkg__",
    ],
    deps = [
        "//winzigc/common:pure_lib",
        "@llvm-project//llvm:Core",
        "@llvm-project//llvm:Support",
    ],
)
