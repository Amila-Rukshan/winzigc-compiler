load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "codegen_lib",
    hdrs = [ 
        "codegen_visitor.h",
        "@llvm-project//llvm:include/llvm/Transforms/InstCombine/InstCombine.h",
        "@llvm-project//llvm:include/llvm/Transforms/InstCombine/InstCombineWorklist.h",
        "@llvm-project//llvm:include/llvm/Transforms/Scalar.h",
        "@llvm-project//llvm:include/llvm/Support/raw_ostream.h",
        "@llvm-project//llvm:include/llvm/Transforms/Utils/SimplifyCFGOptions.h",
        "@llvm-project//llvm:include/llvm/Transforms/Scalar/GVN.h",
        "@llvm-project//llvm:include/llvm/Transforms/Scalar/TailRecursionElimination.h",
        "@llvm-project//llvm:include/llvm/Analysis/InstructionPrecedenceTracking.h",
        "@llvm-project//llvm:include/llvm/Analysis/MemoryDependenceAnalysis.h",
        "@llvm-project//llvm:include/llvm/Analysis/MemoryLocation.h",
        "@llvm-project//llvm:include/llvm/Transforms/Utils.h",
    ],
    srcs = [
        "codegen_visitor.cc",
        "codegen_user_type.cc",
        "codegen_expression.cc",
        "codegen_var.cc",
        "codegen_function.cc",
        "codegen_external.cc",
    ],
    deps = [
        "@com_github_google_glog//:glog",
        "//winzigc/frontend/ast:ast_lib",
        "//winzigc/common:pure_lib",
        "@llvm-project//llvm:Core",
        "@llvm-project//llvm:Support",
        "@llvm-project//llvm:TransformUtils",
        "@llvm-project//llvm:Scalar",
        "@llvm-project//llvm:InstCombine",
    ],
    visibility = ["//winzigc/main:__pkg__"],
)
