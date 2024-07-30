import { assertEquals } from "@std/assert";
import { b, type InferSchema } from "~/sources/mod.ts";

Deno.test("empty object", async () => {
  using handle = await b.init();
  const schema = b.object({});
  const data: InferSchema<typeof schema> = {};
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with string", async () => {
  using handle = await b.init();
  const schema = b.object({ name: b.string() });
  const data: InferSchema<typeof schema> = { name: "a" };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with number", async () => {
  using handle = await b.init();
  const schema = b.object({ age: b.number() });
  const data: InferSchema<typeof schema> = { age: 1 };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with string and number", async () => {
  using handle = await b.init();
  const schema = b.object({ name: b.string(), age: b.number() });
  const data: InferSchema<typeof schema> = { name: "a", age: 1 };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with constrained number", async () => {
  using handle = await b.init();
  const schema = b.object({ age: b.number().min(1).max(5) });
  const data: InferSchema<typeof schema> = { age: 1 };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with constrained string", async () => {
  using handle = await b.init();
  const schema = b.object({ name: b.string().min(1).max(5) });
  const data: InferSchema<typeof schema> = { name: "a" };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with constrained string and number", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string().min(1).max(5),
    age: b.number().min(1).max(5),
  });
  const data: InferSchema<typeof schema> = { name: "a", age: 1 };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with nested object", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string(),
    address: b.object({ city: b.string(), country: b.string() }),
  });
  const data: InferSchema<typeof schema> = {
    name: "a",
    address: { city: "b", country: "c" },
  };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with nested array", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string(),
    friends: b.array(b.string()),
  });
  const data: InferSchema<typeof schema> = { name: "a", friends: ["b", "c"] };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with deeply nested object", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string(),
    address: b.object({
      city: b.string(),
      country: b.string(),
      postalCode: b.object({ code: b.string() }),
    }),
  });
  const data: InferSchema<typeof schema> = {
    name: "a",
    address: { city: "b", country: "c", postalCode: { code: "d" } },
  };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with deeply nested array", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string(),
    friends: b.array(b.object({ name: b.string() })),
  });
  const data: InferSchema<typeof schema> = {
    name: "a",
    friends: [{ name: "b" }, { name: "c" }],
  };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("object with deeply nested object and array", async () => {
  using handle = await b.init();
  const schema = b.object({
    name: b.string(),
    address: b.object({
      city: b.string(),
      country: b.string(),
      postalCode: b.object({ code: b.string() }),
    }),
    friends: b.array(b.object({ name: b.string() })),
  });
  const data: InferSchema<typeof schema> = {
    name: "a",
    address: { city: "b", country: "c", postalCode: { code: "d" } },
    friends: [{ name: "e" }, { name: "f" }],
  };
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});
