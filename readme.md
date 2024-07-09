# blueprint

> [!NOTE]\
> Under construction, please come back later.

```
 ___________________
< speed all the way >
 -------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
```

## usage

Since `blueprint` uses `C++` under the hood for the heavy lifting, the project
needs the following flags to be set when running:

- **--unstable-ffi**: to enable the `FFI` feature.
- **--allow-env**: to read the `BLUEPRINT_PATH` environment variable.
- **--allow-read**: to read the shared library.
- **--allow-write**: to save the shared library.
- **--allow-net**: to download the shared library.
- **--allow-ffi**: to use functions from the shared library.

```sh
deno run \
    --unstable-ffi \
    --allow-env=BLUEPRINT_PATH \
    --allow-read=.,$BLUEPRINT_PATH \
    --allow-write=.,$BLUEPRINT_PATH \
    --allow-net=github.com \
    --allow-ffi \
    sources/example.ts
```
