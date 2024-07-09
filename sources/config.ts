import { sprintf } from "@std/fmt/printf";
import type { SupportedOS } from "~/sources/types.ts";
import config from "~/deno.json" with { type: "json" };

/**
 * Retrieves the file extension for the library based on the current operating system.
 * @returns The file extension for the library.
 * @throws {Error} If the current operating system is not supported.
 */
function getLibraryExtension(): string | undefined {
  const supportedOS: SupportedOS = {
    darwin: "dylib",
    windows: "dll",
    linux: "so",
  };

  if (Deno.build.os in supportedOS) {
    return supportedOS[Deno.build.os];
  }

  throw new Error(sprintf("Unsupported OS: %s", Deno.build.os));
}

/**
 * Returns the library name based on the current operating system.
 * The library name is constructed by combining the prefix, the last part of the config name,
 * and the library extension.
 *
 * @returns The library name.
 */
function getLibraryName(): string {
  const prefix: SupportedOS = {
    darwin: "lib",
    windows: "",
    linux: "lib",
  };

  return sprintf(
    "%s%s-%s.%s",
    prefix[Deno.build.os],
    config.name.split("/").pop(),
    Deno.build.arch,
    getLibraryExtension(),
  );
}

/**
 * Returns the release URL based on the repository name, version tag, and library name.
 * @returns The release URL.
 */
function getReleaseURL(): string {
  const { name, url } = config.repository;
  const tag = sprintf("v%s", config.version);

  return sprintf(url, name, tag, getLibraryName());
}

export { getLibraryName, getReleaseURL };
