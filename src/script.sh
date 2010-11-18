clear
gcc $(pkg-config --libs --cflags gtk+-2.0) lamp-indicator.c -o lamp-indicator
./lamp-indicator
