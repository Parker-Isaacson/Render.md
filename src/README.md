# MarkDown Render Build Instructions

Simple instructions for how to build. Provided builds is statically compiled.

## Windows

For a regular compile.
```PS1
cd .\scr\
g++ --std=c++17 main.cpp utils/html.cpp utils/render.cpp utils/colors.cpp -o windows/mdrender
```

For a static compile.
```PS1
cd .\scr\
g++ --std=c++17 --static main.cpp utils/html.cpp utils/render.cpp utils/colors.cpp -o windows/mdrender
```

## Linux

TBA