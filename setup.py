#! /usr/bin/env python3

import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.test import test as TestCommand
import shutil
import glob


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(
                re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPYTHON_EXECUTABLE=' + sys.executable
        ]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += [
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                    cfg.upper(), extdir)
            ]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''), self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp,
                              env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)
        # Copy *_test file to tests directory
        out_dir = os.path.join(self.build_temp, 'pyksp')
        if not os.path.exists(out_dir):
            print("creating directory {}".format(out_dir))
            os.makedirs(out_dir)

        # Copy file
        src_file = self.get_ext_fullpath(ext.name)
        dest_file = os.path.join(out_dir, os.path.basename(src_file))
        print("copying {} -> {}".format(src_file, dest_file))
        shutil.copy(src_file, dest_file)

        print()  # Add an empty line for cleaner output


with open('requirements.txt') as f:
    install_requires = f.read().splitlines()

setup(
    name='pyksp',
    version='0.2',
    author='Laurent Lejeune',
    author_email='me@lejeunel.org',
    description='Edge-disjoint K-shortest paths',
    long_description='',
    packages=find_packages('src'),
    package_dir={'': 'src'},
    ext_modules=[CMakeExtension('pyksp')],
    install_requires=install_requires,
    cmdclass=dict(build_ext=CMakeBuild),
    test_suite='tests',
    zip_safe=False,
)
