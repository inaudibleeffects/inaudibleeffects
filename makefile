BUNDLE = echoizer.lv2
INSTALL_DIR = /usr/local/lib/lv2

$(BUNDLE): manifest.ttl echoizer.ttl echoizer.so echoizer_ui.so ui/background.png ui/knob.png ui/cursor.png
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

echoizer.so: echoizer.c
	gcc `pkg-config --cflags --libs lv2-plugin` -lm -shared -fPIC -DPIC echoizer.c -o echoizer.so

echoizer_ui.so: echoizer_ui.c
	gcc `pkg-config --cflags --libs gtk+-3.0 lv2-gui` -lm -shared -fPIC -DPIC echoizer_ui.c ui/knob.c -o echoizer_ui.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)/*

clean:
	rm -rf $(BUNDLE) echoizer.so echoizer_ui.so