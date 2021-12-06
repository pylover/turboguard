# turboguard

[![PyPI](http://img.shields.io/pypi/v/turboguard.svg)](https://pypi.python.org/pypi/turboguard)
[![Build](https://github.com/pylover/turboguard/actions/workflows/build.yml/badge.svg)](https://github.com/pylover/turboguard/actions/workflows/build.yml)
[![Coverage Status](https://coveralls.io/repos/github/pylover/turboguard/badge.svg?branch=master)](https://coveralls.io/github/pylover/turboguard?branch=master)

Python C extension to validate and sanitize the user input using blacklist 
and character map.

## Install

```bash
pip install turboguard
```


### Quickstart.

Create an instance of the `Sanitizer` class as the below.

The `Sanitizer.__enter__` method returns a `callable(str) -> str` which let 
you to call it many times without worring about performance and memory leak.

```python
from turboguard import Sanitizer, BlacklistedError


blacklist = [
    ('\U0001d100', '\U0001d1ff'),  # Blacklist Unicode range
    ('\u0600', '\u0610'),          # Blacklist Unicode range
    '\u0635',                      # Blacklist single character
]

replace = [
    ('\u0636', '\u0637'),     # Replace \u0636 by \u0637
]

with Sanitizer(blacklist, replace) as sanitize:    # Loading(Slow) part
    try:
        print(sanitize('foo bar baz'))             # Fast call!
    except BlacklistedError:
        print('Validation failed!')
```

## Contribution

The `turboguard/core.c` file contains all logics for allocation and memory
cleanup as well as the `core_sanitize` function which is the only function 
to use the given database.

`turboguard/__init__.py` just contains the Python wrapper arround the C 
module and force cleanup and initiate using the Python's context manager
(the `with` syntax).

### What to do after fork:

#### Setup development environment

*It's highly recommended to create a virtual environment at the first.*

Install project in editable mode: `pip install -e . `

```bash
make env
```

#### Build the C extension

```bash
make build
```

#### Test your environment

```bash
make cover
```

### What to do after edit:

#### Lint  code using:

```bash
make lint
```

#### Pass tests:

```bash
make clean build cover
```

#### Submit a pull request.
