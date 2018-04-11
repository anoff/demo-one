.PHONY: emcc serve autobuild
build:
	g++ src/**.cpp -lsdl2 -O2 -o dist/demo.o -std=c++11
debug:
	g++ src/**.cpp -lsdl2 -O2 -o dist/demo.o -g -std=c++11
run:
	exec dist/demo.o
dockeremcc:
	docker run \
  --rm \
  -v $(shell pwd):/src \
  -u emscripten \
  trzeci/emscripten-slim \
  em++ src/*.cpp -o dist/out.html -O3 -s USE_SDL=2 --shell-file assets/shell_minimal.html -std=c++11
emcc:
	em++ src/*.cpp -o dist/outwasm.html -O3 -s USE_SDL=2 --shell-file assets/shell_minimal.html -std=c++11 -s WASM=1
autobuild:
	fswatch -e ".*" -i "\\.cpp$$" -i "\\.h$$" . |  xargs -n1 -I{} em++ src/*.cpp -o dist/out.html -O3 -s USE_SDL=2 --shell-file assets/shell_minimal.html -std=c++11
serve:
	live-server dist/ --entry-file=out.html
