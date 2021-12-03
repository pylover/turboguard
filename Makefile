.PHONY: lint
lint:
	flake8

.PHONY: env
env:
	pip install --upgrade pip
	pip install -e .
	pip install -r requirements-dev.txt

.PHONY: cover
cover:
	pytest --cov=strbench tests
