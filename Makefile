.PHONY: emcc serve autobuild
build:
	g++ *.cpp -lsdl2 -O3 -o demo.out
debug:
	g++ *.cpp -lsdl2 -o demo.out -g
run:
	exec ./demo.out
dockeremcc:
	docker run \
  --rm \
  -v $(shell pwd):/src \
  -u emscripten \
  trzeci/emscripten-slim \
  em++ *.cpp -o out.html -O3 -s USE_SDL=2 --shell-file shell_minimal.html
emcc:
	em++ *.cpp -o dist/out.html -O3 -s USE_SDL=2 --shell-file shell_minimal.html
autobuild:
	fswatch -e ".*" -i "\\.cpp$$" -i "\\.h$$" . |  xargs -n1 -I{} em++ *.cpp -o dist/out.html -O3 -s USE_SDL=2 --shell-file shell_minimal.html
serve:
	live-server dist/ --entry-file=out.html
