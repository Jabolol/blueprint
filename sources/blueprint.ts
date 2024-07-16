import { init } from "~/sources/loader.ts";
import { Constraints, type ISchema } from "~/sources/schema.ts";
import type { Blueprint, InferSchema, PayloadMap } from "~/sources/types.ts";

/**
 * Represents the base blueprint class.
 *
 * @example
 * ```ts
 * using handle = await b.init();
 *
 * const schema = b.object({
 *  name: b.string().length(10),
 *  age: b.number().min(18),
 * });
 *
 * const data = {
 *  name: "Javi",
 *  age: 21,
 * }
 *
 * const result = handle.parse(schema, data);
 *
 * console.log(`The schema is ${result ? "valid" : "invalid"}`);
 * ```
 *
 * @extends Constraints
 */
export class b extends Constraints {
  private _handle: Blueprint & Disposable;
  private _parser: Deno.PointerObject<unknown>;

  /**
   * Constructs a new instance of the blueprint class.
   * @param handle - The handle for the blueprint.
   */
  private constructor(handle: Blueprint & Disposable) {
    const parser = handle.create_parser();
    if (parser === null) {
      throw new Error("Failed to create parser");
    }

    super();
    this._handle = handle;
    this._parser = parser;
  }

  /**
   * Initializes the FFI library and returns a new instance of the blueprint class.
   * @returns A promise that resolves to a new instance of the blueprint class.
   */
  static async init() {
    const handle = await init();

    return new this(handle);
  }

  /**
   * Parses the given data using the specified schema.
   * @param schema - The schema to use for parsing.
   * @param data - The data to parse.
   * @returns A boolean indicating whether the parsing was successful.
   */
  parse<T extends ISchema<keyof PayloadMap>>(
    schema: T,
    data: InferSchema<T>,
  ): boolean {
    return this._handle.parse(
      this._parser,
      schema.toPointer(),
      this.toPointer(JSON.stringify(data)),
    );
  }

  /**
   * Disposes the blueprint and releases any resources.
   */
  [Symbol.dispose]() {
    this._handle.destroy_parser(this._parser);
    this._handle[Symbol.dispose]();
  }

  /**
   * Converts the given data to a pointer.
   * @param data - The data to convert.
   * @returns The pointer to the converted data.
   */
  private toPointer(data: string) {
    const bytes = new TextEncoder().encode(data);
    const pointer = Deno.UnsafePointer.of(bytes);
    if (pointer === null) {
      throw new Error("Failed to create pointer");
    }

    return pointer;
  }
}
