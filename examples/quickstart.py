from turboguard import Sanitizer, BlacklistedError


blacklist = [
    ('\U0001d100', '\U0001d1ff'),  # Blacklist Unicode range
    ('\u0600', '\u0610'),          # Blacklist Unicode range
    '\u0635',                      # Blacklist single character
]

replace = [
    ('\u0636', '\u0637'),     # Replace \u0636 by \u0637
    ('b', 'B'),
]

with Sanitizer(blacklist, replace) as sanitize:    # Loading(Slow) part
    try:
        # Fast calls
        assert sanitize('foo bar') == 'foo Bar'    # Fast call!
        assert sanitize(None) is None
    except BlacklistedError:
        print('Validation failed!')
