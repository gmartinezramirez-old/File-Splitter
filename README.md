# File Splitter and joiner

## Compilation

* Execute makefile, to compile the 3 executables: main, CUT and JOIN. The object code will be compiled in the "build / obj" folder and the executables in the "build /" folder.

```
$ make
```

## Execution

* Go to `build/' directory.
* For execution you must run the main program __`main`__ next to the Options __` - cut N <input> `__ if you want to cut the _input_ file in _N_ parts, or with __` - join <original-file> `__ if you want to restore the original file.
* The JOIN program will retrieve the missing part - if any - and unite all the parts to restore the original program.

```
$ ./main --cut N file.extension
```

```
$ ./main --join file.extension

* Example 

```
$ ./main --cut 6 doc.pdf
```

```
$ ./main --join doc.pdf
```
```
