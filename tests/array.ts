import { assertEquals } from "@std/assert";
import { b, type InferSchema } from "~/sources/mod.ts";

Deno.test("array with minimum length", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).min(1);
  const data: InferSchema<typeof schema> = [1];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with maximum length", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).max(1);
  const data: InferSchema<typeof schema> = [1];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with minimum and maximum length", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).min(1).max(2);
  const data: InferSchema<typeof schema> = [1];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with minimum and maximum length (invalid)", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).min(1).max(2);
  const data: InferSchema<typeof schema> = [1, 2, 3];
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("array with allowed values", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).values([1, 2, 3]);
  const data: InferSchema<typeof schema> = [1, 2, 3];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with allowed values (invalid)", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).values([1, 2, 3]);
  const data: InferSchema<typeof schema> = [1, 2, 4];
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("empty array", async () => {
  using handle = await b.init();
  const schema = b.array(b.number()).min(1);
  const data: InferSchema<typeof schema> = [];
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("array with numbers", async () => {
  using handle = await b.init();
  const schema = b.array(b.number());
  const data: InferSchema<typeof schema> = [1, 2, 3, 4, 5];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with strings", async () => {
  using handle = await b.init();
  const schema = b.array(b.string());
  const data: InferSchema<typeof schema> = ["a", "b", "c", "d", "e"];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with objects", async () => {
  using handle = await b.init();
  const schema = b.array(b.object({ name: b.string() }));
  const data: InferSchema<typeof schema> = [{ name: "a" }, { name: "b" }, {
    name: "c",
  }];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with objects and numbers", async () => {
  using handle = await b.init();
  const schema = b.array(b.object({ name: b.string(), age: b.number() }));
  const data: InferSchema<typeof schema> = [
    { name: "a", age: 1 },
    { name: "b", age: 2 },
    { name: "c", age: 3 },
  ];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with constrained numbers", async () => {
  using handle = await b.init();
  const schema = b.array(b.number().min(1).max(5));
  const data: InferSchema<typeof schema> = [1, 2, 3, 4, 5];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with constrained strings", async () => {
  using handle = await b.init();
  const schema = b.array(b.string().min(1).max(5));
  const data: InferSchema<typeof schema> = ["a", "b", "c", "d", "e"];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with constrained objects", async () => {
  using handle = await b.init();
  const schema = b.array(b.object({ name: b.string().min(1).max(5) }));
  const data: InferSchema<typeof schema> = [{ name: "a" }, { name: "b" }, {
    name: "c",
  }];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with constrained objects and numbers (invalid)", async () => {
  using handle = await b.init();
  const schema = b.array(
    b.object({
      name: b.string().min(1).max(5),
      age: b.number().min(1).max(5),
    }),
  ).min(1).max(5);
  const data: InferSchema<typeof schema> = [
    { name: "a", age: 1 },
    { name: "b", age: 2 },
    { name: "c", age: 6 },
  ];
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});

Deno.test("array with constrained objects and numbers (valid)", async () => {
  using handle = await b.init();
  const schema = b.array(
    b.object({
      name: b.string().min(1).max(5),
      age: b.number().min(1).max(5),
    }),
  ).min(1).max(5);
  const data: InferSchema<typeof schema> = [
    { name: "a", age: 1 },
    { name: "b", age: 2 },
    { name: "c", age: 3 },
  ];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with constrained objects and arrays", async () => {
  using handle = await b.init();
  const schema = b.array(
    b.object({
      name: b.string().min(1).max(5),
      age: b.number().min(1).max(5),
      hobbies: b.array(b.string().min(1).max(5)),
      misc: b.object({ a: b.string().min(1).max(5) }),
    }),
  ).min(1).max(5);
  const data: InferSchema<typeof schema> = [
    {
      name: "a",
      age: 1,
      hobbies: ["a", "b", "c"],
      misc: { a: "a" },
    },
    {
      name: "b",
      age: 2,
      hobbies: ["a", "b", "c"],
      misc: { a: "a" },
    },
    {
      name: "c",
      age: 3,
      hobbies: ["a", "b", "c"],
      misc: { a: "a" },
    },
  ];
  const result = handle.verify(schema, data);

  assertEquals(result, true);
});

Deno.test("array with nested constrained objects and nested arrays (invalid)", async () => {
  using handle = await b.init();
  const schema = b.array(b.object({
    name: b.string().min(5).enum(["javi", "jose"]),
    age: b.number().min(18).max(22),
    hobbies: b.array(b.string().min(1).max(5)),
    whatever: b.array(b.object({
      a: b.string().min(1).max(5),
      b: b.number().min(1).max(5),
    })).min(1).max(2),
  })).min(1).max(5);
  const data: InferSchema<typeof schema> = [
    {
      name: "javi",
      age: 18,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
    {
      name: "jose",
      age: 19,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
    {
      name: "javi",
      age: 20,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
    {
      name: "jose",
      age: 21,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
    {
      name: "javi",
      age: 22,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
    {
      name: "pepe",
      age: 23,
      hobbies: ["a", "b", "c"],
      whatever: [{ a: "a", b: 1 }, { a: "b", b: 2 }],
    },
  ];
  const result = handle.verify(schema, data);

  assertEquals(result, false);
});
