PRJ = turboguard

.PHONY: all
all: 
	python setup.py build_ext --inplace

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
	pytest --cov=$(PRJ) tests


.PHONY: clean
clean:
	python setup.py clean
	rm -rf build/*
