include ../Makefile.mk

all: manifest.ttl echoizer.ttl echoizer.so echoizer_ui.so
	rm -rf $(DEST_DIR)
	mkdir $(DEST_DIR)
	cp $^ $(DEST_DIR)

ui: cleanui
	ld -r -b binary -o background.o background.png
	ld -r -b binary -o knob.o knob.png

echoizer.so: echoizer.c
	gcc `pkg-config --cflags --libs lv2-plugin` -lm -shared -fPIC -DPIC echoizer.c -o echoizer.so

echoizer_ui.so: echoizer_ui.c ui
	#gcc `pkg-config --cflags --libs gtk+-3.0 lv2-gui` -lm -shared -fPIC -DPIC echoizer_ui.c background.o cursor.o knob.o -o echoizer_ui.so
	gcc -DPUGL_HAVE_CAIRO \
		-I $(LIBS_DIR) \
	 	-lcairo \
		-lm \
		-lX11 \
		$(LIBS_DIR)/pugl/pugl_x11.c\
		$(LIBS_DIR)/*.c \
		$(LIBS_DIR)/types/*.c \
		$(LIBS_DIR)/widgets/*.c \
		./echoizer_ui.c knob.o background.o -o cairo
#
# install: $(BUNDLE)
# 	mkdir -p $(INSTALL_DIR)
# 	rm -rf $(INSTALL_DIR)/$(BUNDLE)
# 	cp -R $(BUNDLE) $(INSTALL_DIR)
#
# uninstall:
# 	rm -rf $(INSTALL_DIR)/$(BUNDLE)/*

clean:
	rm -rf $(BUNDLE) echoizer.so echoizer_ui.so *.o

cleanui:
	rm -f background.o cursor.o knob.o
