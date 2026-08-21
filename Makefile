all:
	gcc -o dp dp.c `pkg-config --cflags --libs gtk4`
