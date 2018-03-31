# demo fun 🕹

## c++ 

```sh
make build && make run
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