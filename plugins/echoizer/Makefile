include ../Makefile.mk

all: manifest.ttl echoizer.ttl echoizer.so echoizer_ui.so
	rm -rf $(DEST_DIR)
	mkdir $(DEST_DIR)
	cp $^ $(DEST_DIR)

ui: cleanui
	ld -r -b binary -o background.o background.png
	ld -r -b binary -o knob.o knob.png

echoizer.so: echoizer.c
	gcc `pkg-config --cflags --libs lv2-plugin` \
		-lm \
		-shared \
		-fPIC \
		-DPIC echoizer.c \
		-o echoizer.so

echoizer_ui.so: ui
	gcc `pkg-config --cflags --libs lv2-gui` \
		-lm \
		-shared \
		-fPIC \
		-DPIC \
		-DPUGL_HAVE_CAIRO \
		-I $(LIBS_DIR) \
		-lcairo \
		-lm \
		-lX11 \
		$(LIBS_DIR)/*.c \
		$(LIBS_DIR)/types/*.c \
		$(LIBS_DIR)/ui/pugl/pugl_x11.c \
		$(LIBS_DIR)/ui/*.c \
		$(LIBS_DIR)/ui/widgets/*.c \
		echoizer_ui.c background.o knob.o -o echoizer_ui.so

test:
	gcc -DPUGL_HAVE_CAIRO \
		-I $(LIBS_DIR) \
		-lcairo \
		-lm \
		-lX11 \
		$(LIBS_DIR)/*.c \
		$(LIBS_DIR)/types/*.c \
		$(LIBS_DIR)/ui/pugl/pugl_x11.c \
		$(LIBS_DIR)/ui/*.c \
		$(LIBS_DIR)/ui/widgets/*.c \
		test.c knob.o background.o -o test

install: # $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R ../../$(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)/*

clean:
	rm -rf $(BUNDLE) echoizer.so echoizer_ui.so *.o

cleanui:
	rm -f background.o cursor.o knob.o
