# RENDER.md

A program for rendering a markdown file to a html file locally, comes with some customization. This program will accpet either .md files or .mdl ( Markdown Local )

## TODO

- Ensure memory safety
- Build interpreter for markdown files
  - void render(); // Bypass because void render(htmlElement& parent = root_); is not allowed :(

## Supported Elements

The markdown elements that will from [Markdown Cheat Sheet](www.markdownguide.org/cheat-sheet/). The list is also below:
- Headings `# Heading`
- Bold `**bold**`
- Itialic `*italic*`
- Blockquote `> blockquote`
- Lists
  - Unordered `- Item`
- Horizontal Rules `---`
- Link `[title](https://example.com)`
- Image `![alt text](path/to/img.jpg)`

## Unsupported Elements

These markdown elements are currently not supported, though will be added at some point.
- Code `` `code` ``
- Order List `1. Item`
- Tables
- Fenced Code Blocks
- Footnotes
- Heading ID's
- Definition List
- Strike Trough
- Task List
- Highlight
- Subscript
- Superscript

## Restrictions

As of now the syntax will be strict:
- All elements that start a line must be followed by a space, and have no spaces before them.
- No nesting is allowed.
- Horizontal Rules must be the only elemet on a line.
- If two lines are not separated by a blank line then they will be placed into the same paragraph.

## Adjustments

Adjustments are things that change how the renderer will look through the code. Afther the block of adjustments must be at leat 1 blank line. As of now the following are possibe:
- Tab Width
  - This is the amount of spaces 1 Tab will be asumed to have.
  - Can be changed with `--tab-width` or by adding `?tab-width==` to the top of the markdown file.
  - Expects an integer value.
  - Default value of `2`.
- Text Color
  - This will be the color of the text.
  - Can be changed with `--text-color` or by adding `?text-color==` to the top of the markdown file.
  - Expects a string, like `#000000` will work. Spaces are not permited.
  - Default value of of `#000000`.
- Background Color
  - This will be the background color of the output.
    - This will also effect the background color of `` `code` ``, with `` `code` `` being 15% darker than what is provided here.
    - This will also effect the background color of a Blockquote, with a Blockquote being 15% darker than what is provided here.
  - Can be changed with `--background-color` or by adding `?background-color==` to the top of the markdown file.
  - Expects a string, like either `#ffffff` will work. Spaces are not permited.
  - Defualt value of `#ffffff`.
- Margin, *recomended to leave as default*
  - This will determing the amount of margin surrounding all elements.
  - Can be changed with `--margin` or by adding `?margin==` to the top of the markdown file.
  - Expects a integer.
  - Defualt value of `0`.
- Padding, *recomended to leave as default*
  - This will determing the amount of padding surrounding all elements.
  - Can be changed with `--padding` or by adding `?padding==` to the top of the markdown file.
  - Expects a integer.
  - Defualt value of `0`.