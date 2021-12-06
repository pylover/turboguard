import re
from os.path import join, dirname

from setuptools import setup, Extension


# reading package version (same way the sqlalchemy does)
HERE = dirname(__file__)
with open(join(HERE, 'strbench', '__init__.py')) as v_file:
    package_version = re.compile('.*__version__ = \'(.*?)\'', re.S).\
        match(v_file.read()).group(1)


dependencies = []


core = Extension(
    'strbench.core',
    sources=[
        'strbench/core.c',
    ],
    # include_dirs=[join(HERE, 'include')]
)

setup(
    name='sanitizer',
    version=package_version,
    description='sanitize characters',
    author='Ati',
    packages=['strbench'],
    install_requires=dependencies,
    ext_modules=[core],
    # TODO: classifiers
)
