import pytest

from strbench import Sanitizer
from strbench.core import BlacklistedError


def test_pipeline():
    sanitizer = Sanitizer(blacklist=[
        ('h', 'z'),
        '!',
        'ل',
    ])

    # TODO: GIL

    with sanitizer:
        sanitizer.blacklist_check('abcش')

        with pytest.raises(BlacklistedError):
            sanitizer.blacklist_check('haaa')

        with pytest.raises(BlacklistedError):
            sanitizer.blacklist_check('!')
