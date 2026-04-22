# code clone deup detector

## tool arguments
- `-model` - the name of the model to use 
- `-dim` - the dimensionality of the model
- `-threshold` - the similarity threshold of the clone detection
- `-scan-dir` - the directory to scan; tool fetches code clones from files in the given dir and any subdirs

## setup
1. have `ctags` installed
1. download [ollama](https://ollama.com/)
2. download an embedding model
3. compile project
4. all set!

## sample run
- `c2d2 -scan-dir path/to/dir -model embeddinggema -dim 768 -threshold 0.9`

## compilation
1. run the following cmake command from the project root: `cmake -B build -S .; cmake --build build `
1. now program is runnable through `./build/dedup`




## credits:
- ollama.hpp: [https://github.com/jmont-dev/ollama-hpp](https://github.com/jmont-dev/ollama-hpp)
- yagp: [https://github.com/yekyam/YAGP](https://github.com/yekyam/YAGP)
