import { assertEquals } from "@std/assert";
import { b, type InferSchema } from "~/sources/mod.ts";

Deno.test("string with minimum length", async () => {
  using handle = await b.init();
  const schema = b.string().min(5);
  const data: InferSchema<typeof schema> = "hello";
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("string with maximum length", async () => {
  using handle = await b.init();
  const schema = b.string().max(19);
  const data: InferSchema<typeof schema> = "hello";
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("string with exact length", async () => {
  using handle = await b.init();
  const schema = b.string().length(3);
  const data: InferSchema<typeof schema> = "wow";
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("string with minimum and maximum length", async () => {
  using handle = await b.init();
  const schema = b.string().min(2).max(10);
  const data: InferSchema<typeof schema> = "hello";
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("string with minimum and maximum length (invalid)", async () => {
  using handle = await b.init();
  const schema = b.string().min(1).max(2);
  const data: InferSchema<typeof schema> = "abc";
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("string with allowed values", async () => {
  using handle = await b.init();
  const schema = b.string().enum(["hello", "world"]);
  const data: InferSchema<typeof schema> = "world";
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});
