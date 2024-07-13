# blueprint

Fast schema parser using C++ under the hood for performance.

## installation

> [!NOTE]\
> In order to use this package, [`deno`](https://deno.land) is required.

1. Install the package from the `jsr` registry:

```sh
deno add @hello/blueprint
```

2. Import the package in your project:

```ts
import { init } from "@hello/blueprint";
```

3. Initialize the package:

```ts
using handle = await init();
```

After the initialization, the `handle` can be used to interact with the package.
The symbols will be freed automatically when the `handle` goes out of scope.
