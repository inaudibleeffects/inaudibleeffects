include Makefile.mk

all: plugins

plugins:
	$(MAKE) -C $(PLUGINS_DIR)

clean:
	$(MAKE) clean -C $(PLUGINS_DIR)

.PHONY: plugins
