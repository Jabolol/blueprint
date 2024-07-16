import { sprintf } from "@std/fmt/printf";
import type { Blueprint } from "~/sources/types.ts";
import config from "~/deno.json" with { type: "json" };
import { getLibraryName, getReleaseURL } from "~/sources/config.ts";

/**
 * Checks if a file exists at the specified path.
 * @param path - The path to the file.
 * @returns A promise that resolves to a boolean indicating whether the file exists.
 */
async function fileExists(path: string): Promise<boolean> {
  try {
    const { size } = await Deno.lstat(path);

    return size > 0;
  } catch (err) {
    if (err instanceof Deno.errors.NotFound) {
      return false;
    }

    throw err;
  }
}

/**
 * Saves a library from a specified URL to the given path.
 * @param path - The path where the library will be saved.
 * @returns A promise that resolves when the library is successfully saved.
 * @throws {Error} If the library fails to be fetched or saved.
 */
async function saveLibrary(path: string): Promise<void> {
  const url = getReleaseURL();
  const response = await fetch(url);
  if (!response.ok) {
    throw new Error(sprintf("Failed to fetch library from %s", url));
  }

  const buffer = await response.arrayBuffer();
  const folder = path.includes("/")
    ? path.split("/").slice(0, -1).join("/")
    : path;
  const exists = await fileExists(folder);
  if (!exists) {
    await Deno.mkdir(folder, { recursive: true });
  }

  await Deno.writeFile(path, new Uint8Array(buffer));

  const success = await fileExists(path);
  if (!success) {
    throw new Error(sprintf("Failed to save library at %s", path));
  }
}

/**
 * Retrieves the path of the library.
 * If the `BLUEPRINT_PATH` environment variable is set, it will be used as the path.
 * Otherwise, it will use the local path specified in the configuration file.
 * If the library does not exist at the specified path, it will be saved before returning the path.
 *
 * @returns A promise that resolves to the path of the library.
 */
async function getLibraryPath(): Promise<string> {
  const { local } = config.repository;
  const override = Deno.env.get("BLUEPRINT_PATH");
  const path = override ?? sprintf(local, getLibraryName());
  const exists = await fileExists(path);
  if (!exists) {
    await saveLibrary(path);
  }

  return path;
}

/**
 * Initializes the blueprint by loading the library and returning a Blueprint object.
 * @returns A Promise that resolves to a Blueprint object.
 */
async function init(): Promise<Blueprint & Disposable> {
  const path = await getLibraryPath();
  const handle = Deno.dlopen(path, {
    version: { parameters: [], result: "pointer" },
    create_parser: { parameters: [], result: "pointer" },
    destroy_parser: { parameters: ["pointer"], result: "void" },
    parse: { parameters: ["pointer", "pointer", "pointer"], result: "bool" },
  });

  const versionPointer = handle.symbols.version();
  if (versionPointer === null) {
    throw new Error(sprintf("Failed to get version from %s", path));
  }

  const view = new Deno.UnsafePointerView(versionPointer);
  const version = view.getCString();
  if (version !== config.version) {
    throw new Error(
      sprintf(
        "Expected v%s, but got v%s. Please delete %s or supply a path to the correct library.",
        config.version,
        version,
        path,
      ),
    );
  }

  return {
    ...handle.symbols,
    [Symbol.dispose]() {
      handle.close();
    },
  };
}

export { init };
