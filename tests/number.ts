import { assertEquals } from "@std/assert";
import { b, type InferSchema } from "~/sources/mod.ts";

Deno.test("number with minimum value", async () => {
  using handle = await b.init();
  const schema = b.number().min(5);
  const data: InferSchema<typeof schema> = 10;
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("number with maximum value", async () => {
  using handle = await b.init();
  const schema = b.number().max(19);
  const data: InferSchema<typeof schema> = 10;
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("number with minimum and maximum value", async () => {
  using handle = await b.init();
  const schema = b.number().min(2).max(10);
  const data: InferSchema<typeof schema> = 5;
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("number with minimum and maximum value (invalid)", async () => {
  using handle = await b.init();
  const schema = b.number().min(1).max(2);
  const data: InferSchema<typeof schema> = 3;
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("number with allowed values", async () => {
  using handle = await b.init();
  const schema = b.number().values([1, 2, 3]);
  const data: InferSchema<typeof schema> = 2;
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("number with allowed values (invalid)", async () => {
  using handle = await b.init();
  const schema = b.number().values([1, 2, 3]);
  const data: InferSchema<typeof schema> = 4;
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});
