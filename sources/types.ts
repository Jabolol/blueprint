import type {
  ArraySchema,
  NumberSchema,
  ObjectSchema,
  StringSchema,
} from "~/sources/schema.ts";

/**
 * Represents the supported operating systems.
 * The keys of this object are the different operating systems supported by Deno.
 * The values are optional strings that provide additional information about each supported OS.
 */
export type SupportedOS = {
  [key in typeof Deno.build.os]?: string;
};

/**
 * Represents a Blueprint object.
 * @property version - A function that returns the version of the library.
 * @property create_parser - A function that creates a new parser.
 * @property destroy_parser - A function that destroys an existing parser.
 * @param parser - A pointer to the parser to be destroyed.
 * @property parse - A function that parses the given data using the provided schema.
 * @param parser - A pointer to the parser to be used.
 * @param schema - A pointer to the schema to be used.
 * @param data - A pointer to the data to be parsed.
 */
export type Blueprint = {
  version: () => Deno.PointerValue;
  create_parser: () => Deno.PointerValue;
  destroy_parser: (parser: Deno.PointerValue) => void;
  parse: (
    parser: Deno.PointerValue,
    schema: Deno.PointerValue,
    data: Deno.PointerValue,
  ) => boolean;
};

/**
 * Ensures that at least one property of type T is required.
 * @template T - The type to enforce the constraint on.
 */
export type AtLeastOne<T> = {
  [K in keyof T]-?: Required<Pick<T, K>> & Partial<Omit<T, K>>;
}[keyof T];

/**
 * Represents the common payload structure.
 * @property REQUIRED - Indicates if the payload is required.
 */
type CommonPayload = {
  "REQUIRED": boolean;
};

/**
 * Represents the internal payload map for different types.
 * @property number - The payload structure for numbers.
 * @property string - The payload structure for strings.
 * @property object - The payload structure for objects.
 * @property array - The payload structure for arrays.
 */
type InternalPayloadMap = {
  number: {
    "MIN_VALUE": number;
    "MAX_VALUE": number;
  };
  string: {
    "MIN_LENGTH": number;
    "MAX_LENGTH": number;
    "ENUM": string[];
  };
  object: {
    "ENUM": string[];
  };
  array: {
    "VALUES": unknown[];
    "MIN_LENGTH": number;
    "MAX_LENGTH": number;
  };
};

/**
 * Represents the payload map, combining common payload properties with specific payload types.
 * @template key - The key of the payload map.
 */
export type PayloadMap = {
  [key in keyof InternalPayloadMap]: CommonPayload & InternalPayloadMap[key];
};

/**
 * Represents a payload for a given type.
 * @template T - The type to get the payload for.
 */
export type Payload<T> = T extends keyof PayloadMap ? AtLeastOne<
    Partial<PayloadMap[T]>
  >
  : never;

/**
 * Extracts the keys of an object type.
 * @template U - The object type to extract keys from.
 */
export type Keys<U> = U extends Record<infer K, unknown> ? K : never;

/**
 * Represents all possible constraints as a map.
 * @template k - The keys of the payload map.
 */
export type AllConstraints = {
  [k in Keys<PayloadMap[keyof PayloadMap]>]: number;
};

/**
 * Infers the schema type from the given schema.
 * @template T - The schema type to infer from.
 */
export type InferSchema<T> = T extends NumberSchema ? number
  : T extends StringSchema ? string
  : T extends ObjectSchema<infer U> ? {
      [K in keyof U]: InferSchema<U[K]>;
    }
  : T extends ArraySchema<infer U> ? InferSchema<U>[]
  : never;
