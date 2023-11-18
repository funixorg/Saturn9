
ifeq ($(OS),Windows_NT)
	CURRENT_DIR := $(shell echo %cd%)
else
	CURRENT_DIR := $(shell pwd)
endif

all: dockerrun

build:
	python3 -u scripts/build.py

docker:
	docker build -t saturn9-env .

dockerrun:
	docker run -it --rm -v "$(CURRENT_DIR)/bin:/usr/src/saturn9/bin/" saturn9-env

run:
	python3 -u scripts/run.py

buildrun: build run