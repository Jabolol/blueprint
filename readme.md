# blueprint

Fast schema parser using C++ under the hood for performance.

## installation

> [!NOTE]\
> In order to use this package, an architecture of `windows-x86_64`,
> `linux-x86_64`, `macos-x86_64` or `macos-aarch64` is required.

1. Install the package from the `jsr` registry:

```sh
deno add @hello/blueprint
```

2. Import the package in your project:

```ts
import { b } from "@hello/blueprint";
```

3. Initialize the parser:

```ts
using handle = await b.init();
```

4. Create a schema:

```ts
const schema = b.object({
  name: b.string(),
  age: b.number().min(18),
});
```

5. Create data to parse:

```ts
const data = {
  name: "Javi",
  age: 21,
};
```

6. Parse the data:

```ts
const result = b.parse(schema, data);
```
