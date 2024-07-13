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
 */
export type Blueprint = {
  version: () => Deno.PointerValue;
  create_parser: () => Deno.PointerValue;
  destroy_parser: (parser: Deno.PointerValue) => void;
};
