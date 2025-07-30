#  Example Files for Rendering

This will contain differnet example files. They should be testing with respect to the adjustments at the top. 

All files here are manually written ( as of now ), so be warry of human error.

Defaults look like the following:
```html
<!-- Base Markdown <div> Will Look Like This -->
<div style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;" id="markdown">
    <!-- All Headings Will Look Like This -->
    <h1 style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;">
        Heading
    </h1>

    <!-- All <p> Will Look Like This -->
    <p style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;">
        Paragraph
    </p>

    <!-- All Blockquotes Will Look Like This -->
    <div style="background-color: ?background-color * 0.85; color: ?text-color; padding: ?padding; margin: ?margin; border-left: 4px solid ?background-color * 0.7; border-radius: 4px; padding-left: 2px;">
        <p style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;">
            Paragraph
        </p>
    </div>

    <!-- All Lists Will Look Like This -->
    <ul style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;">
        <li style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;">I am an element of an unordered list</li>
        <!-- Nesting Follows the Same Patern -->
    </ul>
    
    <!-- All Code Will Look Like This -->
    <div style="background-color: ?background-color; color: ?text-color; padding: max(2px, ?padding); margin: ?margin; width: fit-content;">
        <p style="background-color: ?background-color; color: ?text-color; padding: ?padding; margin: ?margin;  width: fit-content;">
            Code
        </p>
    </div>

    <!-- Bold / Italic -->
    <!-- Surround words with either <b> or <i> -->
</div>
```