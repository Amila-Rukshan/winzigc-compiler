load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cmd",
    srcs = ["cmd.cc"],
    deps = [
        "@com_github_google_glog//:glog",
        "//winzigc/frontend/lexer:lexer_lib",
        "//winzigc/frontend/parser:parser_lib",
        "//winzigc/visitor:codegen_lib",
    ],
)
