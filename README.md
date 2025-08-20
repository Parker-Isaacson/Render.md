# RENDER.md

RENDER.md renders markdown files into html files locally, allowing for use for accelerated web development.

## Supported Elements

The markdown elements that will from [Markdown Cheat Sheet](www.markdownguide.org/cheat-sheet/).
- Headings `# Heading`
  - ID's `# Heading {#id}`
- Bold `**bold**`
- Itialic `*italic*`
- Blockquote `> blockquote`
- Lists
  - Unordered `- Item`
  - Task `- [ ] Task`
- Horizontal Rules `---`
- Link `[title](https://example.com)`
- Image `![alt text](path/to/img.jpg)`
- Strike Trough `~~The world is flat.~~`
- Subscript `H~2~O`
- Superscript `x^2^`
- Highlight `==very important words==`
- Code `` `code` ``
- Fenced Code Blocks ```` ```code``` ````

## Unsupported Elements

These markdown elements are currently not supported, though will be added at some point.
- Order List `1. Item`
- Tables
- Footnotes
- Definition List

## Restrictions

Restrictions to how this will compile are strict.
- All elements that start a line must be followed by a space, and have no spaces before them.
- No nesting is allowed.
- Horizontal Rules must be the only elemet on a line. Along with the start and end for Fenced Code Blocks.
- Heading ID's must be at the end of the line.

## Adjustments

Adjustments are things that change how the renderer will look through the code.
- Tab Width
  - This is the amount of spaces 1 Tab will be asumed to have.
  - Can be changed by adding `?tab-width==` to the top of the markdown file.
  - Expects an integer value.
  - Default value of `2`.
- Text Color
  - This will be the color of the text.
  - Can be changed by adding `?text-color==` to the top of the markdown file.
  - Expects a string, like `#000000` will work. Spaces are not permited.
  - Default value of of `#000000`.
- Background Color
  - This will be the background color of the output.
    - This will also effect the background color of `` `code` ``, with `` `code` `` being 15% darker than what is provided here.
    - This will also effect the background color of a Blockquote, with a Blockquote being 15% darker than what is provided here.
  - Can be changed by adding `?background-color==` to the top of the markdown file.
  - Expects a string, like either `#ffffff` will work. Spaces are not permited.
  - Defualt value of `#ffffff`.
- Margin, *recomended to leave as default*
  - This will determing the amount of margin surrounding all elements.
  - Can be changed by adding `?margin==` to the top of the markdown file.
  - Expects a integer.
  - Defualt value of `0`.
- Padding, *recomended to leave as default*
  - This will determing the amount of padding surrounding all elements.
  - Can be changed by adding `?padding==` to the top of the markdown file.
  - Expects a integer.
  - Defualt value of `0`.

## Parameters

Parameters that the program will accept when executed.
- `--help`
  - Brings up the help message and retunrs
- `--input-file <file>`
  - Sets the input file
  - Has a defualt value of `"input.md"`
- `--output-file <file>`
  - Sets the output file
  - Has a defualt value of `"markdown.html"`