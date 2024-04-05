load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "cmd",
    srcs = ["cmd.cc"],
    deps = [":main_lib"],
)

cc_library(
    name = "main_lib",
    hdrs = ["cmd.h"],
    visibility = [
        "//test/integration:__pkg__",
    ],
    deps = [
        "//winzigc/frontend/lexer:lexer_lib",
        "//winzigc/frontend/parser:parser_lib",
        "//winzigc/visitor/codegen:codegen_lib",
        "//winzigc/visitor/semantic:semantic_lib",
        "@com_github_google_glog//:glog",
    ],
)
