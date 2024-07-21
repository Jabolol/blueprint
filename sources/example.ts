import { b, type InferSchema } from "~/sources/mod.ts";

using handle = await b.init();
const schema = b.array(b.number()).min(1);
const data: InferSchema<typeof schema> = [];
const result = handle.verify(schema, data);

if (!result) {
    console.log({ error: handle.error });
}
