all:
	gcc `pkg-config --cflags --libs gtk+-3.0` -lm echoizer.c ../knob.c background.o cursor.o knob.o -o ui
	#gcc -o ui `pkg-config --cflags --libs gtk+-3.0` ../knob.c echoizer.c -lm

clean:
	rm ui
