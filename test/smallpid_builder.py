#! /usr/bin/python3

from distutils.core import setup, Extension

# the c++ extension module
extension_mod = Extension("SmallPID",
                          sources = ["smallpid_wrapper.cpp"],
                          include_dirs = ['../src'],
                          extra_compile_args = ["-std=c++20"])

setup(name = "SmallPID",
      ext_modules=[extension_mod],
      zip_safe=False,
      )


# test
#import SmallPID
