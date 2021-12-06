import re
from os.path import join, dirname

from setuptools import setup, Extension


# reading package version (same way the sqlalchemy does)
HERE = dirname(__file__)
with open(join(HERE, 'turboguard', '__init__.py')) as v_file:
    package_version = re.compile('.*__version__ = \'(.*?)\'', re.S).\
        match(v_file.read()).group(1)


dependencies = []


core = Extension(
    'turboguard.core',
    sources=['turboguard/core.c']
)

setup(
    name='sanitizer',
    version=package_version,
    description='sanitize characters',
    author='Ati',
    packages=['turboguard'],
    install_requires=dependencies,
    ext_modules=[core],
    # TODO: classifiers
)
