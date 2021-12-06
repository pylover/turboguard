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
        s.blacklist_check('abcش')

        with pytest.raises(BlacklistedError):
            s.blacklist_check('haaa')

        with pytest.raises(BlacklistedError):
            s.blacklist_check('!')
