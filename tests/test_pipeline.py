from strbench import sanitize


def test_pipeline():
    assert sanitize('foo') == 'foo'
