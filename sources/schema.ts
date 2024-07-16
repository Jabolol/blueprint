import { sprintf } from "@std/fmt/printf";
import type {
  AllConstraints,
  InferSchema,
  Keys,
  Payload,
  PayloadMap,
} from "~/sources/types.ts";

/**
 * Represents the available constraints for a schema.
 */
const FLAGS: AllConstraints = {
  MIN_VALUE: 1 << 0,
  MAX_VALUE: 1 << 1,
  MIN_LENGTH: 1 << 2,
  MAX_LENGTH: 1 << 3,
  ENUM: 1 << 4,
  REQUIRED: 1 << 5,
  VALUES: 1 << 6,
};

/**
 * Represents an abstract class for a schema.
 * @template T - The type of the payload map.
 */
export abstract class ISchema<T extends keyof PayloadMap> {
  private _bitmap: number = 0;
  private _constraints: Payload<T>[] = [];

  /**
   * Adds a constraint to the schema.
   * @param constraint - The constraint to add.
   * @returns The instance of the schema.
   * @throws Error if the constraint is already set.
   */
  protected addConstraint(constraint: Payload<T>): this {
    const keys = Object.keys(constraint) as Keys<
      PayloadMap[keyof PayloadMap]
    >[];
    for (const key of keys) {
      if (this.has(key)) {
        throw new Error(sprintf("Constraint %s already set", key));
      }
      this.set(key, true);
      this._constraints.push(constraint);
    }

    return this;
  }

  private has(id: Keys<PayloadMap[keyof PayloadMap]>): boolean {
    const bits = FLAGS[id];

    return (this._bitmap & bits) === bits;
  }

  private set(id: Keys<PayloadMap[keyof PayloadMap]>, on: boolean): void {
    if (on) {
      this._bitmap |= FLAGS[id];
    } else {
      this._bitmap &= ~FLAGS[id];
    }
  }

  /**
   * Converts the schema to an object.
   * @returns The schema as an object.
   */
  public toObject(): object {
    return this._constraints;
  }

  /**
   * Converts the schema to a string.
   * @returns The schema as a string.
   */
  public toString(): string {
    return JSON.stringify(this.toObject());
  }

  /**
   * Converts the schema to a pointer value.
   * @returns The schema as a pointer value.
   */
  public toPointer(): Deno.PointerValue {
    const data = this.toString();
    const bytes = new TextEncoder().encode(data);
    const pointer = Deno.UnsafePointer.of(bytes);

    return pointer;
  }
}

/**
 * Represents an object schema.
 * @template T - The type of the object schema.
 */
export class ObjectSchema<T extends Record<string, ISchema<keyof PayloadMap>>>
  extends ISchema<"object"> {
  private _data: T;

  /**
   * Creates a new instance of ObjectSchema.
   * @param data - The data for the object schema.
   */
  constructor(data: T) {
    super();
    this._data = data;
  }

  /**
   * Converts the object schema to an object.
   * @returns The object representation of the schema.
   */
  override toObject(): object {
    const object: Record<string, object> = {};
    for (const [key, value] of Object.entries(this._data)) {
      object[key] = value.toObject();
    }

    return object;
  }

  /**
   * Adds an enum constraint to the object schema.
   * @param values - The allowed values for the enum.
   * @returns The updated object schema.
   */
  enum(values: string[]): this {
    return this.addConstraint({ ENUM: values });
  }
}

/**
 * Represents an array schema.
 * @template T - The type of the array elements.
 */
export class ArraySchema<T extends ISchema<keyof PayloadMap>> extends ISchema<
  "array"
> {
  private _data: T;

  /**
   * Creates a new instance of ArraySchema.
   * @param data - The data for the array schema.
   */
  constructor(data: T) {
    super();
    this._data = data;
  }

  /**
   * Converts the array schema to an object.
   * @returns The object representation of the array schema.
   */
  override toObject(): object {
    return this._data.toObject();
  }

  /**
   * Sets the minimum length constraint for the array schema.
   * @param value - The minimum length value.
   * @returns The current instance of ArraySchema.
   */
  min(value: number): this {
    return this.addConstraint({ MIN_LENGTH: value });
  }

  /**
   * Sets the maximum length constraint for the array schema.
   * @param value - The maximum length value.
   * @returns The current instance of ArraySchema.
   */
  max(value: number): this {
    return this.addConstraint({ MAX_LENGTH: value });
  }

  /**
   * Sets the allowed values constraint for the array schema.
   * @param values - The allowed values.
   * @returns The current instance of ArraySchema.
   */
  values(values: [InferSchema<T>, ...InferSchema<T>[]]): this {
    return this.addConstraint({ VALUES: values });
  }
}

/**
 * Represents a schema for validating numbers.
 */
export class NumberSchema extends ISchema<"number"> {
  /**
   * Sets the minimum value constraint for the number.
   * @param value The minimum value to set.
   * @returns The updated NumberSchema instance.
   */
  min(value: number): this {
    return this.addConstraint({ MIN_VALUE: value });
  }

  /**
   * Sets the maximum value constraint for the number.
   * @param value The maximum value to set.
   * @returns The updated NumberSchema instance.
   */
  max(value: number): this {
    return this.addConstraint({ MAX_VALUE: value });
  }
}

/**
 * Represents a string schema.
 */
export class StringSchema extends ISchema<"string"> {
  /**
   * Sets the length constraint for the string.
   * @param value The length value.
   * @returns The updated string schema.
   */
  length(value: number): this {
    return this.addConstraint({ MIN_LENGTH: value, MAX_LENGTH: value });
  }

  /**
   * Sets the minimum length constraint for the string.
   * @param value The minimum length value.
   * @returns The updated string schema.
   */
  minLength(value: number): this {
    return this.addConstraint({ MIN_LENGTH: value });
  }

  /**
   * Sets the maximum length constraint for the string.
   * @param value The maximum length value.
   * @returns The updated string schema.
   */
  maxLength(value: number): this {
    return this.addConstraint({ MAX_LENGTH: value });
  }

  /**
   * Sets the enum constraint for the string.
   * @param values The enum values.
   * @returns The updated string schema.
   */
  enum<T extends string>(values: [T, ...T[]]): this {
    return this.addConstraint({ ENUM: values });
  }
}

/**
 * Abstract class representing constraints for defining schemas.
 */
export abstract class Constraints {
  /**
   * Creates a new object schema with the specified data.
   * @param data The data defining the object schema.
   * @returns An instance of the ObjectSchema class.
   */
  static object<T extends Record<string, ISchema<keyof PayloadMap>>>(data: T) {
    return new ObjectSchema(data);
  }

  /**
   * Creates a new array schema with the specified data.
   * @param data The data defining the array schema.
   * @returns An instance of the ArraySchema class.
   */
  static array<T extends ISchema<keyof PayloadMap>>(data: T) {
    return new ArraySchema(data);
  }

  /**
   * Creates a new number schema.
   * @returns An instance of the NumberSchema class.
   */
  static number() {
    return new NumberSchema();
  }

  /**
   * Creates a new string schema.
   * @returns An instance of the StringSchema class.
   */
  static string() {
    return new StringSchema();
  }
}
