import pytest

from strbench import Sanitizer
from strbench.core import BlacklistedError


def test_blacklist():
    blacklist = [
        ('h', 'z'),
        '!',
        '\u0635',
    ]

    with Sanitizer(blacklist) as s:
        s('abcش')

        with pytest.raises(BlacklistedError):
            s('haaa')

        with pytest.raises(BlacklistedError):
            s('!')


def test_replace():
    replace = [
        ('a', 'A')
    ]

    with Sanitizer(replace=replace) as s:
        assert s('abcada') == 'AbcAdA'


def test_mixed():
    blacklist = [
        ('b', 'f')
    ]
    replace = [
        ('a', 'A')
    ]

    with Sanitizer(blacklist, replace) as s:
        with pytest.raises(BlacklistedError):
            s('b')

        assert s('ahaia') == 'AhAiA'
        assert s('') == ''
