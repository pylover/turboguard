from .sanitizer import sanitize
from . import core


__version__ = '0.1.0'


class Sanitizer:
    _handle = None

    def __init__(self, blacklist=None):
        self.blacklist = blacklist or []

    def _import_blacklist(self):
        """Slow method!"""
        for item in self.blacklist:
            if not isinstance(item, tuple):
                start, end = item, item
            else:
                start, end = item

            core.blacklist_append(self._handle, ord(start), ord(end))

    def blacklist_check(self, string):
        core.blacklist_check(self._handle, string)

    def __enter__(self):
        blacklistsize = len(self.blacklist)
        self._handle = core.create(blacklistsize)
        self._import_blacklist()

    def __exit__(self, extype, exvalue, traceback):
        core.dispose(self._handle)
        self._handle = None
