import { b, type InferSchema } from "~/sources/mod.ts";
import { assertEquals } from "@std/assert";

Deno.test("empty array", async () => {
    using handle = await b.init();
    const schema = b.array(b.number()).min(1);
    const data: InferSchema<typeof schema> = [];
    const result = handle.verify(schema, data);

    // FIXME(jabolo): This case fails
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
