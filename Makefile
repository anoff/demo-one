build:
	g++ *.cpp -lsdl2 -O3 -o demo.out
debug:
	g++ *.cpp -lsdl2 -o demo.out -g
run:
	exec ./demo.out
docker-emcc:
	docker run \
  --rm \
  -v $(shell pwd):/src \
  -u emscripten \
  trzeci/emscripten-slim \
  em++ *.cpp -o out.html -O3 -s USE_SDL=2 --shell-file shell_minimal.html
