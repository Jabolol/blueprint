/**
 * Represents the supported operating systems.
 * The keys of this object are the different operating systems supported by Deno.
 * The values are optional strings that provide additional information about each supported OS.
 */
export type SupportedOS = {
  [key in typeof Deno.build.os]?: string;
};

/**
 * Represents an object that can be disposed.
 */
export type Dispose = {
  [Symbol.dispose]: () => void;
};

/**
 * Represents a Blueprint object.
 * @property {Function} hello - A function that prints a greeting message.
 */
export type Blueprint = {
  hello: () => void;
} & Dispose;
