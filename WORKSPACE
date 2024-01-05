# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Demonstration of using these build files with http_archive instead of submodules.

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

################################################################
# llvm setup

LLVM_COMMIT = "499bce3abab8a362b9b4197944bd40b826c736c4"

LLVM_BAZEL_TAG = "llvm-project-%s" % (LLVM_COMMIT,)

LLVM_BAZEL_SHA256 = "a05a83300b6b4d8b45c9ba48296c06217f3ea27ed06b7e698896b5a3b2ed498d"

http_archive(
    name = "llvm-bazel",
    sha256 = LLVM_BAZEL_SHA256,
    strip_prefix = "llvm-bazel-{tag}/llvm-bazel".format(tag = LLVM_BAZEL_TAG),
    url = "https://github.com/google/llvm-bazel/archive/{tag}.tar.gz".format(tag = LLVM_BAZEL_TAG),
)

LLVM_SHA256 = "a154965dfeb2b5963acc2193bc334ce90b314acbe48430ba310d8a7c7a20de8b"

LLVM_URLS = [
    "https://storage.googleapis.com/mirror.tensorflow.org/github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT),
    "https://github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT),
]

http_archive(
    name = "llvm-project-raw",
    build_file_content = "#empty",
    sha256 = LLVM_SHA256,
    strip_prefix = "llvm-project-" + LLVM_COMMIT,
    urls = LLVM_URLS,
)

load("@llvm-bazel//:terminfo.bzl", "llvm_terminfo_disable")

llvm_terminfo_disable(
    name = "llvm_terminfo",
)

load("@llvm-bazel//:zlib.bzl", "llvm_zlib_disable")

llvm_zlib_disable(
    name = "llvm_zlib",
)

load("@llvm-bazel//:configure.bzl", "llvm_configure")

llvm_configure(
    name = "llvm-project",
    src_path = ".",
    src_workspace = "@llvm-project-raw//:WORKSPACE",
)

################################################################
################################################################
# glog setup

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)

################################################################
################################################################
