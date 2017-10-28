include Makefile.mk

all: plugins

clean:
	$(MAKE) clean -C $(PLUGINS_DIR)

install:
	$(MAKE) install -C $(PLUGINS_DIR)

plugins:
	$(MAKE) -C $(PLUGINS_DIR)

uninstall:
	$(MAKE) uninstall -C $(PLUGINS_DIR)

.PHONY: plugins
