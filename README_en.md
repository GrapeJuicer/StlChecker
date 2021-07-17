**日本語**版のREADMEは[こちら](https://github.com/GrapeJuicer/StlChecker/blob/main/README.md)．

# StlChecker 1.0.0 (EN)

StlChecker is a program to check if .STL files in text/binary format have the same structure.

## Feature
- Checks for two `.STL` files.
- Checks for differences in the organization of the files, up to the order in which they are written to.
- Both binary and text formats are supported.
- Can check between text and binary.

## Usage
```
$ stlc [options...] file1 file2
```
Note: It doesn't matter where you write the `options`.


### Examples:

```
$ stlc -h
$ stlc --version
$ stlc binary1.stl binary2.stl
$ stlc text.stl binary.stl --format=tb
$ stlc text1.stl text2.stl -f tt -r
$ stlc binary.stl text.stl -rfbt
etc...
```

## Options

| Short option                  | Long option                                | About                                                                                                      |
| :---------------------------- | :----------------------------------------- | :--------------------------------------------------------------------------------------------------------- |
| -h                            | --help                                     | Display command information.                                                                               |
| -v                            | --version                                  | Display command version information.                                                                       |
| -f`<t/b...>`<br>-f `<t/b...>` | --format=`<t/b...>`<br>--format `<t/b...>` | Select file format. You can select Text(t) or Binary(b).<br>Ex: file1=text, file2=binary >>> `--format=tb` |
| -r                            | --result                                   | Display only if there is a match. It returns 'True' or 'False'.                                            |


### To be supported in the future (subject to change)
- Function to check an arbitrary number of files simultaneously.
- Function to check without considering the order of writing.
- Function to allow for value deviation
