load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cmd",
    srcs = ["cmd.cc"],
    deps = [
        "@com_github_google_glog//:glog",
        "//lexer:lexer"
    ],
)
