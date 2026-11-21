import {
  mkdtempSync,
  readFileSync,
  readdirSync,
  renameSync,
  rmSync,
  statSync,
  writeFileSync
} from 'node:fs'
import { tmpdir } from 'node:os'
import { dirname, extname, join, resolve } from 'node:path'
import { fileURLToPath } from 'node:url'
import { spawnSync } from 'node:child_process'

const root = resolve(dirname(fileURLToPath(import.meta.url)), '..')
const docsDirectory = join(root, 'docs')
const sourceDirectory = join(root, 'scripts', 'fonts')
const outputDirectory = join(docsDirectory, '.vitepress', 'theme', 'fonts')
const temporaryDirectory = mkdtempSync(join(tmpdir(), 'asumoranda-fonts-'))
const textFile = join(temporaryDirectory, 'site-text.txt')

const textExtensions = new Set([
  '.c',
  '.cpp',
  '.css',
  '.cts',
  '.h',
  '.hpp',
  '.html',
  '.ino',
  '.js',
  '.json',
  '.md',
  '.mdx',
  '.mjs',
  '.mts',
  '.pl',
  '.ps1',
  '.py',
  '.scss',
  '.sh',
  '.tex',
  '.toml',
  '.ts',
  '.txt',
  '.vue',
  '.xml',
  '.yaml',
  '.yml'
])

const ignoredDirectories = new Set([
  '.git',
  '.vitepress/cache',
  '.vitepress/dist',
  'public'
])
const fontNames = [
  'GenInterfaceJP-Regular.woff2',
  'MoralerspaceNeonHW-Regular.woff2',
  'ZenKakuGothicNew-Regular.woff2'
]

function collectTextFiles(directory, relativeDirectory = '') {
  const files = []

  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    const relativePath = join(relativeDirectory, entry.name)
    if (entry.isDirectory()) {
      if (!ignoredDirectories.has(relativePath.replaceAll('\\', '/'))) {
        files.push(
          ...collectTextFiles(join(directory, entry.name), relativePath)
        )
      }
      continue
    }

    if (
      entry.isFile() &&
      textExtensions.has(extname(entry.name).toLowerCase())
    ) {
      files.push(join(directory, entry.name))
    }
  }

  return files
}

try {
  const siteText = collectTextFiles(docsDirectory)
    .map((file) => readFileSync(file, 'utf8'))
    .join('\n')

  writeFileSync(textFile, siteText, 'utf8')

  for (const fontName of fontNames) {
    const sourceFont = join(sourceDirectory, fontName)
    const outputFont = join(outputDirectory, fontName)
    const temporaryFont = `${outputFont}.tmp`

    if (!statSync(sourceFont, { throwIfNoEntry: false })?.isFile()) {
      throw new Error(`Font source not found: ${sourceFont}`)
    }

    rmSync(temporaryFont, { force: true })
    const result = spawnSync(
      'pyftsubset',
      [
        sourceFont,
        `--output-file=${temporaryFont}`,
        `--text-file=${textFile}`,
        '--unicodes=U+0000-00FF,U+2000-206F,U+20A0-20CF,U+3000-30FF,U+FF00-FFEF',
        '--layout-features=*',
        '--flavor=woff2'
      ],
      { stdio: 'inherit' }
    )

    if (result.error?.code === 'ENOENT') {
      throw new Error(
        'pyftsubset is required. Install Python fonttools before building.'
      )
    }
    if (result.status !== 0) {
      throw new Error(`pyftsubset failed for ${fontName}`)
    }

    renameSync(temporaryFont, outputFont)
    console.log(
      `${fontName}: ${(statSync(outputFont).size / 1024).toFixed(1)} KiB`
    )
  }
} finally {
  rmSync(temporaryDirectory, { recursive: true, force: true })
}
