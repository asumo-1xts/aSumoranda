from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory


ROOT = Path(__file__).resolve().parent.parent
DOCS_DIRECTORY = ROOT / "docs"
SOURCE_DIRECTORY = ROOT / "scripts" / "fonts"
OUTPUT_DIRECTORY = DOCS_DIRECTORY / ".vitepress" / "theme" / "fonts"

TEXT_EXTENSIONS = {
    ".c",
    ".cpp",
    ".css",
    ".cts",
    ".h",
    ".hpp",
    ".html",
    ".ino",
    ".js",
    ".json",
    ".md",
    ".mdx",
    ".mjs",
    ".mts",
    ".pl",
    ".ps1",
    ".py",
    ".scss",
    ".sh",
    ".tex",
    ".toml",
    ".ts",
    ".txt",
    ".vue",
    ".xml",
    ".yaml",
    ".yml",
}
IGNORED_DIRECTORIES = {
    Path(".git"),
    Path(".vitepress/cache"),
    Path(".vitepress/dist"),
    Path("public"),
}
FONT_NAMES = (
    "GenInterfaceJP-Regular.woff2",
    "MoralerspaceNeonHW-Regular.woff2",
    "ZenKakuGothicNew-Regular.woff2",
)


def collect_text_files(directory: Path) -> list[Path]:
    files = []

    for entry in directory.rglob("*"):
        if not entry.is_file() or entry.suffix.lower() not in TEXT_EXTENSIONS:
            continue

        relative_path = entry.relative_to(DOCS_DIRECTORY)
        if any(
            relative_path == ignored or ignored in relative_path.parents
            for ignored in IGNORED_DIRECTORIES
        ):
            continue

        files.append(entry)

    return files


def main() -> None:
    OUTPUT_DIRECTORY.mkdir(parents=True, exist_ok=True)

    with TemporaryDirectory(prefix="asumoranda-fonts-") as temporary_directory:
        text_file = Path(temporary_directory) / "site-text.txt"
        site_text = "\n".join(
            file.read_text(encoding="utf-8")
            for file in collect_text_files(DOCS_DIRECTORY)
        )
        text_file.write_text(site_text, encoding="utf-8")

        for font_name in FONT_NAMES:
            source_font = SOURCE_DIRECTORY / font_name
            output_font = OUTPUT_DIRECTORY / font_name
            temporary_font = output_font.with_name(f"{output_font.name}.tmp")

            if not source_font.is_file():
                raise FileNotFoundError(f"Font source not found: {source_font}")

            temporary_font.unlink(missing_ok=True)
            try:
                subprocess.run(
                    [
                        "pyftsubset",
                        str(source_font),
                        f"--output-file={temporary_font}",
                        f"--text-file={text_file}",
                        "--layout-features=kern,liga,calt",
                        "--flavor=woff2",
                    ],
                    check=True,
                )
            except FileNotFoundError as error:
                raise RuntimeError(
                    "pyftsubset is required. Install the fonttools dependency."
                ) from error
            except subprocess.CalledProcessError as error:
                raise RuntimeError(f"pyftsubset failed for {font_name}") from error

            temporary_font.replace(output_font)
            print(f"{font_name}: {output_font.stat().st_size / 1024:.1f} KiB")


if __name__ == "__main__":
    try:
        main()
    except (FileNotFoundError, RuntimeError) as error:
        print(error, file=sys.stderr)
        sys.exit(1)
