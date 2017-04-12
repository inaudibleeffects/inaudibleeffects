BUNDLE = echoizer.lv2
INSTALL_DIR = /usr/local/lib/lv2

$(BUNDLE): manifest.ttl echoizer.ttl echoizer.so echoizer_ui.so ui/background.png ui/knob.png ui/cursor.png
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

echoizer.so: echoizer.c
	gcc `pkg-config --cflags --libs lv2-plugin` -lm -shared -fPIC -DPIC echoizer.c -o echoizer.so

echoizer_ui.so: echoizer_ui.c
	ld -r -b binary -o background.o ui/background.png
	ld -r -b binary -o cursor.o ui/cursor.png
	ld -r -b binary -o knob.o ui/knob.png
	gcc `pkg-config --cflags --libs gtk+-3.0 lv2-gui` -lm -shared -fPIC -DPIC echoizer_ui.c ui/knob.c background.o cursor.o knob.o -o echoizer_ui.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)/*

clean:
	rm -rf $(BUNDLE) echoizer.so echoizer_ui.so *.o

all: clean echoizer.so echoizer_ui.so uninstall install
