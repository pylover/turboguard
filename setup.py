from setuptools import setup
import re
from os.path import join, dirname


# reading package version (same way the sqlalchemy does)
with open(join(dirname(__file__), 'strbench', '__init__.py')) as v_file:
    package_version = re.compile('.*__version__ = \'(.*?)\'', re.S).\
        match(v_file.read()).group(1)


dependencies = []


setup(
    name='sanitizer',
    version=package_version,
    description='sanitize characters',
    auther='Ati',
    packages=['strbench'],
    # TODO: classifiers
    install_requiers=dependencies,
)
