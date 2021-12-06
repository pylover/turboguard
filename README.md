# turboguard

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
    ('\u1d100', '\u1d1ff'),   # Blacklist Unicode range
    '\u0635',                 # Blacklist single character
    ...
]

replace = [
    ('\u0636', '\u0637'),     # Replace \u0636 by \u0637
    ...
]

with Sanitizer(blacklist, replace) as sanitize:    # Loading(Slow) part
    try:
      print(sanitize('foo bar baz'))                 # Fast call!
    except BlacklistedError:
      print('Validation failed!')
```

## Contribution

```bash
make env
make build
make cover
```

Afterward, for development sycle:

```bash
make clean build cover
```
