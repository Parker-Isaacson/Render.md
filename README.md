# MarkDown Render

A program for rendering a markdown file to a html file locally, comes with some customization

## TODO

- Ensure memory safety
- Build interpreter for markdown files

## Supported Elements

The markdown elements that will from [Markdown Cheat Sheet](www.markdownguide.org/cheat-sheet/). The list is also below:
- Headings `# Heading`
- Bold `**bold**`
- Itialic `*italic*`
- Blockquote `> blockquote`
- Lists
  - Ordered `1. Item`
  - Unordered `- Item`
- Code `` `code` ``
- Horizontal Rules `---`
- Link `[title](https://example.com)`
- Image `![alt text](path/to/img.jpg)`

## Restrictions

As of now the syntax will be strict:
- All elemts that start a line must be followed by a space.
- Headings must start the line with no white space before them, up to 6 \# are permited for `<h1>` through `<h6>` respectively.
- `**Bold**` and `*Italic*` charters are expected to be "touching" the word they are next to, though they can surround multiple.
- Combinations of `***Bold and Italic***` are possible but must follow the same rules as above and use 3 asterisk.
- Blockqutoes \> must start the line, a depth of 1 block quotes is the limit.
- Lists must start the line, to have nested lists 2 spaces are needed, this can be changed by adjusting `tab_width`.
- Horizontal Rules must start the line and be the only elemet on a line.
- Links and Images must be padded with at least 1 space around them.
- A single blank line will insert a `<br>`, see example 1. If there is no blank line text will be placed in the same `<p>` tag.

## Adjustments

Adjustments are things that change how the renderer will look through the code as of now the following are possibe:
- Tab Width
  - This is the amount of spaces 1 Tab will be asumed to have.
  - Can be changed with `--tab-width` or by adding `?tab-width==` to the top of the file that is being rendered.
  - Expects an integer value.
  - Default value of `2`
- Text Color
  - This will be the color of the text.
  - Can be changed with `--text-color` or by adding `?text-color==` to the top of the file that is being rendered.
  - Expects a string, like either `black` or `#000000` will work. Spaces are not permited.
  - Default value of of `#000000`
- Background Color
  - This will be the background color of the output
    - This will also effect hte background color of `` `code` ``, with `` `code` `` being 15% darker than what is provided here.
  - Can be changed with `--background-color` or by adding `?background-color==` to the top of the file that is being rendered.
  - Expects a string, like either `black` or `#000000` will work. Spaces are not permited.
  - Defualt value of `#ffffff`


## Examples

This section will provide an example of markdown followed by how it will be rendered to html.

### Example 1

Markdown in the form of:

```markdown
Paragraph 1

Paragraph 2
```

Will render to:

```html
<p>Paragraph 1</p>
<br>
<p>Paragraph 2</p>
```
