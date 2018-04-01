# demo fun 🕹

## c++ 

```sh
make build && make run
```

## dev workflow (auto preview)

For ease of development the following commands will automatically build ASM.js output every time sources change and serve them to a web browser.

```sh
# build automatically if .cpp or .h files change using emscripten
make autobuild
# start up a live-reload server (npm i -g live-server)
make serve
```

## emscripten

### code

Make sure that the main loop is handled by emscripten and not automatically started in `main()`

```c++
  #ifdef __EMSCRIPTEN__
    // 0 fps means to use requestAnimationFrame; non-0 means to use setTimeout.
    emscripten_set_main_loop(loop, 0, 1);
  #else
    while (!quit) {
      loop();
      SDL_Delay(16);
    }
  #endif
```

### setup
Install emscripten

```sh
brew install emscripten
```

```sh
# build html (and js)
em++ *.cpp -o out.html -s USE_SDL=2 --shell-file shell_minimal.html

# run stuff
open out.html
```

## stuff

regex to match **NOT** file ext `^(.(?!(\.cpp|\.h$)))*$`

regex to match **ONLY** file ext `/(.*?)\.(h|cpp)$/`

fswatch for cpp files: `fswatch -e ".*" -i "\\.cpp$" -i "\\.h$" .`