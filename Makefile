
ifeq ($(OS),Windows_NT)
	CURRENT_DIR := $(shell echo %cd%)
else
	CURRENT_DIR := $(shell pwd)
endif

ifeq ($(OS),Windows_NT)
	PYTHON := python
else
	PYTHON := python3
endif

all: docker

build:
	python3 -u scripts/build.py

docker:
	docker build -t saturn9-env .
	docker run -it --rm -v "$(CURRENT_DIR)/bin:/usr/src/saturn9/bin/" -v "$(CURRENT_DIR)/isodir:/usr/src/saturn9/isodir/" saturn9-env

run:
	$(PYTHON) -u scripts/run.py

buildrun: 
	make docker
	make run