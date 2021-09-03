**日本語**版のREADMEは[こちら](https://github.com/GrapeJuicer/StlChecker/blob/main/README.md)．

# StlChecker 1.0.0 (EN)

StlChecker is a program to check if STL files in text/binary format have the same structure.


## Feature
- Checks for two `STL` files.
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
Display help                                      $ stlc -h
Display version                                   $ stlc --version
Compare 2 binary                                  $ stlc binary1.stl binary2.stl
Compare text and binary                           $ stlc text.stl binary.stl --format=tb
Compare 2 text and Display only result            $ stlc text1.stl text2.stl -f tt -r
Compare 2 binary with tolerance 1.5, vertex rule  $ stlc binary1.stl binary2.stl -t1.5 --rule vertex
```

## Options
| Short option                  | Long option                                    | About                                                                                                                                                                                                                                                               |
| :---------------------------- | :--------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| -h                            | --help                                         | Display command information.                                                                                                                                                                                                                                        |
| -v                            | --version                                      | Display command version information.                                                                                                                                                                                                                                |
| -f`<t/b...>`<br>-f `<t/b...>` | --format=`<t/b...>`<br>--format `<t/b...>`     | Select file format. You can select Text(t) or Binary(b).<br>Ex: file1=text, file2=binary >>> `--format=tb`                                                                                                                                                          |
| -r                            | --result                                       | Display only if there is a match. It returns 'True' or 'False'.                                                                                                                                                                                                     |
| -t`<value>`<br>-t `<value>`   | --tolerance=`<value>`<br>--tolerance `<value>` | Set tolerance. <br>Note: Tolerance deviations may occur due to calculation errors.                                                                                                                                                                                  |
| -u`<rule>`<br>-u `<rule>`     | --rule=`<rule>`<br>--rule `<rule>`             | Set the calculation method when the tolerance is set. The following arguments can be given to `<rule>`.<br>`0`, `c`, `component` ... Calculate the distance for each component of a vertex<br>`1`, `v`, `vertex` ... Calculate by the distance between two vertexes |


## Version
| Version | Update     | About                              |
| :------ | :--------- | :--------------------------------- |
| v1.0.0  | 2021/07/17 | First release                      |


### To be supported in the future (subject to change)
- Function to check an arbitrary number of files simultaneously.
- Function to check without considering the order of writing.
- Function to allow for value deviation
