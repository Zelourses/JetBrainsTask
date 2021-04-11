# JetbrainsTask

My realization of JetBrains task to internship on project "Улучшение интеграции Rider с Unreal Editor"

## How to run it

1. just run `git clone --recursive https://github.com/Zelourses/JetBrainsTask.git`  
Next, you need to download/install [PreMake](https://premake.github.io/)  
And now, just run `premake5 <buildoption>` where `<buildoption>` is:

| `<buuildoption>` | description |
| ----------------- | --------------- |
| vs2019 | For MS Visual Studio 2019 |
| vs2017 | For MS Visual Studio 2017 |
| vs2015 | For MS Visual Studio 2015 |
| vs2013 | For MS Visual Studio 2013 |
| vs2012 | For MS Visual Studio 2012 |
| vs2010 | For MS Visual Studio 2010 |
| vs2008 | For MS Visual Studio 2008 |
| vs2005 | For MS Visual Studio 2005 |
| gmake2 | For GNU Makefiles |
| xcode4 | For XCode project |
| codelite | For Codelite project |

I recommend using vs20XX for windows and gmake2 for Linux/Apple  
2. After that, you'll get files to build project (`Makefile`, `Visual studio solution`, e.t.c).  
Then, run something, that will build your project(`make`, `Visual studio`, `Xcode`, e.t.c)
3. You'll get executable file in `out/Release-JetBrainsTask-x64/JetBrainsTask`  
Before you run this file, you need to place near it file named `words.txt` with words to search for.  
Otherwise error window will appear when you start search.

## Configuration

This project has 2 configurations:

- `Debug` - project with console window and debug symbols
- `Release` - project without debug symbols, with optimization on and without console window

## Remarks

1. Originally i was planning to create menu with possibility to choose file, but in Jetbrains task clearly written:  
    **Внешний вид**  
    2 UI элемента:

    - однострочное поле для ввода текста;
    - многострочное поле для вывода текста.

2. In optional tasks we have: "Не хранить словарь в памяти процесса" and my program fullfils this condition.
