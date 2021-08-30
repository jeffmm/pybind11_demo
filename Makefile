.PHONY: help, dev-start, dev-stop

CONTAINER_NAME="pybind11_tutorial_develop_${USER}"

PROJECT=pybind11_tutorial

COMPOSE_FILE=docker/docker-compose.yml

# takes advantage of the makefile structure (command; ## documentation)
# to generate help
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.env: ## make an .env file
	touch .env

dev-start: .env ## Primary make command for devs, spins up containers
	docker-compose -f $(COMPOSE_FILE) --project-name $(PROJECT) up -d --no-recreate

dev-stop: ## Spin down active containers
	docker-compose -f $(COMPOSE_FILE) --project-name $(PROJECT) down

# Useful when Dockerfile/requirements are updated)
dev-rebuild: .env ## Rebuild images for dev containers
	docker-compose -f $(COMPOSE_FILE) --project-name $(PROJECT) up -d --build

bash: dev-start ## Provides an interactive bash shell in the container
	docker exec -it $(CONTAINER_NAME) bash

nb: ## Opens Jupyterlab in the browser
	docker port $(CONTAINER_NAME) | grep 8888 | awk -F ":" '{print "http://localhost:"$$2}' | xargs open
