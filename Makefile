export CC = emcc
export CFLAGS = -O3
export AR = emar

tetrominic.js:
	$(MAKE) -C src tetrominic.js
	mv src/tetrominic.js lib/
	mv src/tetrominic.wasm lib/