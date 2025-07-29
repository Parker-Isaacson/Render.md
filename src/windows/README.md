# MarkDown Render Windows Build

This is the directory for windows build. As other files are added these commands will be updated or changed to a makefile.

The following can be used to build and executed, while inside the windows directory and in powershell:

```PS1
g++ --std=c++17 main.cpp utils/html.cpp -o build/mdrender
.\build\mdrender.exe
```

If you are using command prompt:

```CMD
g++ --std=c++17 main.cpp utils/html.cpp -o build/mdrender
biuld\mdrender.exe
```