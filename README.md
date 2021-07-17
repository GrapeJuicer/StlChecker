*The **English** version of README is [here](https://github.com/GrapeJuicer/StlChecker/blob/main/README_en.md).*

# StlChecker 1.0.0 (JP)

StlCheckerは，テキスト/バイナリ形式の `.STL` ファイルが同じ構造であるかどうかをチェックするプログラムです．

## 特徴
- 2つの `.STL` ファイルに対してチェックを行います．
- ファイルへの書き込み順に至るまでのファイルの構成の違いをチェックします．
- バイナリ形式とテキスト形式の両方に対応．
- テキスト/バイナリ間でのチェックが可能．

## 使用方法
```
$ stlc [options...] file1 file2
```
メモ: `options`はどこに書いても問題ありません．


### 使用例:

```
$ stlc -h
$ stlc --version
$ stlc binary1.stl binary2.stl
$ stlc text.stl binary.stl --format=tb
$ stlc text1.stl text2.stl -f tt -r
$ stlc binary.stl text.stl -rfbt
etc...
```

## オプション一覧

| Short option                  | Long option                                | About                                                                                                                                      |
| :---------------------------- | :----------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------- |
| -h                            | --help                                     | コマンドのヘルプを表示します．                                                                                                             |
| -v                            | --version                                  | コマンドのバージョン情報を表示します．                                                                                                     |
| -f`<t/b...>`<br>-f `<t/b...>` | --format=`<t/b...>`<br>--format `<t/b...>` | フォーマットを指定します．テキスト形式(t)とバイナリ形式(b)が指定できます．<br>例: file1:テキスト形式，file2:バイナリ形式 >>> `--format=tb` |
| -r                            | --result                                   | 一致したかどうかのみを表示します．一致:True / 不一致:False                                                                                   |


### 今後対応予定(変更の可能性あり)
- 任意の数のファイルを同時にチェックする機能．
- 書き込み順を考慮せずにチェックを行う機能．
- 誤差を許容する機能．
