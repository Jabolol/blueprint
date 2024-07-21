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
 * const result = handle.verify(schema, data);
 *
 * console.log(`The schema is ${result ? "valid" : "invalid"}`);
 * ```
 *
 * @extends Constraints
 */
export class b extends Constraints {
  private _error: string | null = null;
  private _handle: Blueprint & Disposable;
  private _blueprint: Deno.PointerObject<unknown>;

  private constructor(handle: Blueprint & Disposable) {
    super();
    this._handle = handle;

    const pointer = this._handle.create();
    if (pointer === null) {
      throw new Error("Failed to create blueprint");
    }

    this._blueprint = pointer;
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
   * Verifies the given data using the specified schema.
   * @param schema - The schema to use for parsing.
   * @param data - The data to verify.
   * @returns A boolean indicating whether the parsing was successful.
   */
  verify<T extends ISchema<keyof PayloadMap>>(
    schema: T,
    data: InferSchema<T>,
  ): boolean {
    const valid = this._handle.verify(
      this._blueprint,
      schema.toPointer(),
      this.toPointer(JSON.stringify(data)),
    );

    if (!valid) {
      const error = this._handle.error(this._blueprint);
      if (error === null) {
        throw new Error("Failed to get error message");
      }

      const view = new Deno.UnsafePointerView(error);
      this._error = view.getCString();
    }

    return valid;
  }

  /**
   * Disposes the blueprint and releases any resources.
   */
  [Symbol.dispose]() {
    this._handle.destroy(this._blueprint);
    this._handle[Symbol.dispose]();
  }

  private toPointer(data: string) {
    const bytes = new TextEncoder().encode(data + "\0");
    const pointer = Deno.UnsafePointer.of(bytes);
    if (pointer === null) {
      throw new Error("Failed to create pointer");
    }

    return pointer;
  }

  /**
   * Gets the error message if the last operation failed
   * @returns The error message if the last operation failed, otherwise `null`.
   */
  public get error() {
    return this._error;
  }
}
