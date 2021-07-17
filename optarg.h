#pragma once

// インクルード ---------------------------------------
#include <getopt.h> // struct option, getopt_long(), opterr, optind
#include <string.h> // strchr()

// 定義 -----------------------------------------------

#define OPT_END {0, 0, 0, 0}

// 構造体 ---------------------------------------------

struct optarg
{
    int opt;
    char *arg;
};

// プロトタイプ宣言 -----------------------------------

int optprocess(
    int argc,                      // 引数の個数
    char *argv[],                  // 引数の内容
    const char *shortopts,         // オプション群(短)
    const struct option *longopts, // オプション群(長)
    struct optarg findopts[],      // 見つかったオプションと引数の情報を格納する場所
    int len_findopts               // findoptsの配列の長さ
);
