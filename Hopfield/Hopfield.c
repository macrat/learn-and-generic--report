﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


const char* PATTERN_NAMES[] = {  /* パターンファイルのファイル名一覧 */
	"crow",
	"dog",
	"duck",
	"lion",
	"monkey",
	"mouse",
	"penguin"
};


#define PATTERN_NUM		(sizeof(PATTERN_NAMES) / sizeof(char*))
#define PATTERN_WIDTH	20
#define PATTERN_HEIGHT	20
#define PATTERN_SIZE	(PATTERN_WIDTH * PATTERN_HEIGHT)

#define TRY_NUM			2  /* 想起の実行回数 */
#define OUTPUT_LEVEL	1  /* 出力の詳細さ。0なら入力と出力だけ、1なら想起一回ごとの出力、2なら1ビットごとの出力。 */

/* 表示に使う文字 */
#if 1 && (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__))  /* *NIXならカラフルに表示しようとする。先頭の1を0にして無効化。 */
	#include <unistd.h>
	#define TRUE_BLOCK	(isatty(fileno(stdout)) ? "\e[37m\e[47m# \e[0m" : "##")
	#define FALSE_BLOCK	(isatty(fileno(stdout)) ? "\e[40m. \e[0m" : "--")
#else  /* *NIXじゃなければ文字だけで表示する */
	#define TRUE_BLOCK	"##"
	#define FALSE_BLOCK	"--"
#endif


/** 出力関数（ステップ関数）
 * 出力関数として使うためのステップ関数。
 * 引数で与えられた内部状態netが正なら1を、負なら-1を返す。
 * 0の場合は前回の出力の値であるpoutを返す。
 * 
 * net: 内部状態の値。
 * pout: 1時刻前の出力の値。
 *
 * return: 内部状態から計算されたニューロンの出力値。
 */
int step_func(const int net, const int pout){
	if(net > 0){
		return 1;
	}else if(net < 0){
		return -1;
	}else{
		return pout;
	}
}


/** パターンの表示
 * 引数で与えられたintの配列を横PATTERN_WIDTH、縦PATTERN_HEIGHTのパターンとして表示する。
 * 表示に使用する文字はTRUE_BLOCKとFALSE_BLOCKの二つ。
 *
 * out: ニューロンの出力値。1か-1のどちらかの配列。
 */
void display_pattern(const int out[PATTERN_SIZE]){
	int x, y;

	for(y=0; y<PATTERN_HEIGHT; y++){
		for(x=0; x<PATTERN_WIDTH; x++){
			printf(out[y*PATTERN_WIDTH + x] > 0 ? TRUE_BLOCK : FALSE_BLOCK);
		}
		printf("\n");
	}
	printf("\n");
}


/** パターンファイルの読み込み
 * 引数で与えられた名前のファイルを開き、一次元のint配列に読み込む。
 * ファイルはクリアテキストで、スペースもしくは改行区切り。
 * 実際のファイルの長さに関わらずPATTERN_SIZE分読み込んだ時点で終了する。
 *
 * ファイルが存在しない場合はエラーを表示したあとにプログラムを終了させる。
 * また、データがPATTERN_SIZEに足りない場合の動作は未定義である。
 *
 * fname: 読み込むファイルの名前。
 * dest: 読み込んだパターンを保存する配列。
 */
void read_pattern(const char *fname, int dest[PATTERN_SIZE]){
	int i;
	FILE *fp;

	/* ファイル fname をオープン */
	if((fp = fopen(fname,"r")) == NULL){
		printf("read_pattern(): Cannot open \"%s\"\n", fname);
		exit(1);
	}

	/* データの読み込み */
	for(i=0; i<PATTERN_SIZE; i++){
		fscanf(fp, "%d", &dest[i]);
	}

	/* ファイルをクローズ */
	fclose(fp);
}


/** 全てのパターンファイルの読み込み
 * グローバルで定義された配列PATTERN_NAMESに列挙されたパターンファイルを全て読み込む。
 *
 * dest: パターンを保存するバッファ。
 */
void read_patterns(int dest[PATTERN_NUM][PATTERN_SIZE]){
	int i;

	for(i=0; i<sizeof(PATTERN_NAMES)/sizeof(char*); i++){
		read_pattern(PATTERN_NAMES[i], dest[i]);
	}
}


/** 複数の入力パターンから重みを決定する
 * パターンの配列を受け取り、ホップフィールドネットワークの重みを決定する。
 *
 * patterns: 学習する入力パターンの配列。1か-1の値を取る。
 * weight: 学習結果を保存する先。weight[i][j]はニューロンiからニューロンjへの重みを示す。
 */
void learn(
		const int patterns[PATTERN_NUM][PATTERN_SIZE],
		int weight[PATTERN_SIZE][PATTERN_SIZE]
){
	int pattern_id, i, j;

	/* weightを初期化 */
	for(i=0; i<PATTERN_SIZE; i++){
		for(j=0; j<PATTERN_SIZE; j++){
			weight[i][j] = 0;
		}
	}

	/* 重みの計算 */
	for(pattern_id=0; pattern_id<PATTERN_NUM; pattern_id++){
		for(i=0; i<PATTERN_SIZE; i++){
			for(j=0; j<PATTERN_SIZE; j++){
				if(i == j){
					weight[i][j] = 0;
				}else{
					weight[i][j] += patterns[pattern_id][i] * patterns[pattern_id][j];
				}
			}
		}
	}
}


/** パターンにノイズを発生させる
 * 入力されたパターンの各ビットについてlevelの確率でデータを反転させる。
 *
 * pattern: 出力先。levelに応じたノイズが乗る。
 * level: 各ビットが反転する確率。0から1の値。0ならなにもせず、1ならすべてが反転する。
 */
void make_noise(int pattern[PATTERN_SIZE], const double level){
	int i;

	for(i=0; i<PATTERN_SIZE; i++){
		if(((double)rand()/RAND_MAX) < level){
			pattern[i] = pattern[i] > 0 ? -1 : 1;
		}
	}
}


/** 想起
 * 与えられた重みと入力から想起を行ない、結果を配列に格納する。
 *
 * weight: 想起に使用する重み。1か-1のいずれかの値の配列。
 * pattern: 入力データ兼出力の保存先。
 * show_progress: 0以外なら途中経過を出力する。
 */
void remember(
		const int weight[PATTERN_SIZE][PATTERN_SIZE],
		int pattern[PATTERN_SIZE],
		const int show_progress
){
	int i, j, net;

	for(i=0; i<PATTERN_SIZE; i++){
		net = 0;
		for(j=0; j<PATTERN_SIZE; j++){
			net += weight[i][j] * pattern[j];
		}
		pattern[i] = step_func(net, pattern[i]);

		/* 出力パターンを表示する */
		if(show_progress){
			display_pattern(pattern);
		}
	}
}


/** 想起の点数を計算
 * 想起にどの程度成功しているかの点数を計算して返却する。
 *
 * output: ホップフィードが出力したパターン。
 * answer: 正解のパターン。
 *
 * return: 想起の成功度合い。0から1までの数値。
 */
double calc_score(const int output[PATTERN_SIZE], const int answer[PATTERN_SIZE]){
	int result = 0;
	int i;

	for(i=0; i<PATTERN_SIZE; i++){
		if(output[i] == answer[i]){
			result++;
		}
	}

	return (double)result/PATTERN_SIZE;
}


/** メイン関数
 * 引数で入力するパターンのIDと発生させるノイズの量を受け取り、計算結果を表示する。
 * 想起の処理はTRY_NUM回繰り返し行なわれる。
 */
int main(const int argc, const char *argv[]){
	int pattern[PATTERN_NUM][PATTERN_SIZE];  /* 学習パターン */
	int weight[PATTERN_SIZE][PATTERN_SIZE];  /* 重み */
	int out[PATTERN_SIZE];  /* 出力 */	 
	int input_id;  /* 入力パターンの番号 */ 
	double noise_level;  /* ノイズレベル */	
	int loop = 1;
	double score = 0;
	int i, j;

	if(argc <= 2){
		fprintf(stderr, "usage: %s [INPUT ID] [NOISE LEVEL] (LOOP NUM)\n", argv[0]);
		fprintf(stderr, "\n");
		fprintf(stderr, "INPUT ID: input pattern ID (0 - %lu)\n", PATTERN_NUM-1);
		for(i=0; i<PATTERN_NUM; i++){
			fprintf(stderr, "\t%d: %s\n", i, PATTERN_NAMES[i]);
		}
		fprintf(stderr, "NOISE LEVEL: noise level (0 - 100[%%])\n");
		fprintf(stderr, "LOOP NUM: if given it, calc average of score.\n");
		return -1;
	}

	input_id = atoi(argv[1]);
	noise_level = atof(argv[2]) / 100.0;
	if(argc > 3){
		loop = atoi(argv[3]);
	}

	/* 引数が正しい範囲に収まっているかを確認。 */
	if(input_id < 0 || PATTERN_NUM <= input_id){
		fprintf(stderr, "input pattern ID is out of range.\n");
		return -1;
	}
	if(noise_level < 0.0 || 1.0 < noise_level){
		fprintf(stderr, "noise level is out of range.\n");
		return -1;
	}
	if(loop < 1){
		fprintf(stderr, "loop num must 1 or more.\n");
		return -1;
	}

	srand(time(NULL));  /* 乱数生成器の初期化。 */

	read_patterns(pattern);  /* 学習パターンの読み込み。 */
	learn((const int (*)[PATTERN_SIZE])pattern, weight);  /* 相関学習 */

	for(i=0; i<loop; i++){
		memcpy(out, pattern[input_id], PATTERN_SIZE * sizeof(int));  /* 入力パターンを出力用の配列にコピーする。 */
		make_noise(out, noise_level);  /* 入力パターンにノイズを乗せる。 */

		if(loop == 1){
			display_pattern(out);  /* 入力パターンを表示する。 */
		}

		/* TRY_NUMの回数分だけ想起処理を行なう。 */
		for(j=0; j<TRY_NUM; j++){
			remember((const int (*)[PATTERN_SIZE])weight, out, OUTPUT_LEVEL >= 2);

			if(OUTPUT_LEVEL >= 1 && loop == 1){
				display_pattern(out);  /* 各想起ごとの出力を表示する。 */
			}
		}

		if(OUTPUT_LEVEL == 0 && loop == 1){
			display_pattern(out);  /* 最終的な出力を表示する。 */
		}

		score += calc_score(out, pattern[input_id]);
	}

	printf("score: %0.2lf%%\n", score/loop*100);

	return 0;
}
