load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "semantic_lib",
    srcs = [
        "semantic_visitor.cc",
    ],
    hdrs = [
        "semantic_visitor.h",
    ],
    visibility = ["//winzigc/main:__pkg__"],
    deps = [
        "//winzigc/common:pure_lib",
        "//winzigc/frontend/ast:ast_lib",
        "@com_github_google_glog//:glog",
    ],
)
