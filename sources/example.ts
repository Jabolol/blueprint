import { b, type InferSchema } from "~/sources/mod.ts";

using handle = await b.init();

const schema = b.object({
  name: b.string().length(10),
  age: b.number().min(18),
  likes: b.array(b.string()).values(["dogs", "cats"]).max(5),
  pets: b.array(b.object({
    name: b.string().length(10),
    age: b.number().min(0),
    kind: b.string().enum(["dog", "cat", "bird"]),
  })).max(3),
});

const data: InferSchema<typeof schema> = {
  name: "Javi",
  age: 21,
  likes: ["cats", "birds"],
  pets: [
    { name: "Lupita", age: 12, kind: "dog" },
  ],
};

console.log(schema);

const result = handle.parse(schema, data);

console.log(`The schema is ${result ? "valid" : "invalid"}`);
