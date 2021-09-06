#
#  Makefile
#

ifeq ($(OS),Windows_NT)
	RM = del
else
	RM = rm -f
endif


# 最終目的のファイル
TARGET = stlc.exe # 実行ファイル名----------------------------

# ソースファイル(*.c)の一覧
CSRCS = optarg.c # ソース ----------------------------
CPPSRCS = main.cpp obj.cpp # ソース ----------------------------

# オブジェクトファイル(*.o)の一覧
COBJS = ${CSRCS:.c=.o}
CPPOBJS = $(CPPSRCS:.cpp=.o)

# ヘッダファイルの一覧
CHEADERS = optarg.h # ヘッダ ----------------------------
CPPHEADERS = obj.hpp # ヘッダ ----------------------------

# コンパイラ・リンカの指定
CC = g++ # コンパイラ
CCFLAGS = -Wall # オプション
LD = g++ # 実行ファイル生成時のコンパイラ
LDFLAGS = -Wall
LIBS = # 引数で与えるライブラリ

# OBJSからTARGETを作る方法
$(TARGET) : $(COBJS)  $(CPPOBJS)
	$(LD) $(COBJS) $(CPPOBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

# *.c から *.o を作る方法
.c.o :
	$(CC) $(CCFLAGS) -c $<

.cpp.o :
	$(CC) $(CCFLAGS) -c $<

# *.oは HEADERS と Makefile に依存(これらが書き終わったときにも*.oを再構築)
$(COBJS) : $(CHEADERS) Makefile
$(CPPOBJS) : $(CPPHEADERS) Makefile

# make cleanとしたときに実行されるコマンド
clean :
	$(RM) $(TARGET) $(COBJS) $(CPPOBJS) *~
