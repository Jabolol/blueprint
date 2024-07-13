import { init } from "~/sources/mod.ts";

using handle = await init();

const object = {
  username: "hello",
  password: "world",
  age: 20,
  interests: {
    programming: true,
    languages: ["rust", "typescript", "c++"],
  },
  friends: [
    {
      name: "Alice",
      age: 21,
    },
    {},
    [12, 13, 14],
  ],
};

const bytes = new TextEncoder().encode(JSON.stringify(object));
const _pointer = Deno.UnsafePointer.of(bytes);

const parser = handle.create_parser();
if (parser === null) {
  throw new Error("Failed to create parser");
}

// parser -> Blueprint::JSON::Parser*

// handle->verify(parser, _pointer, $schema);

handle.destroy_parser(parser);
