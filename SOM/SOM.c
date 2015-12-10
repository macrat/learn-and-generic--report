#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define INPUT_DATA_NUM 16  /* 入力されるデータの数 */
#define INPUT_DATA_LENGTH 29  /* 入力層のニューロン数 */
#ifndef MAP_SIDE_LENGTH
	#define MAP_SIDE_LENGTH 10  /* マップ層の1辺のニューロン数 */
#endif
#define TRAINING_NUM 2000  /* 学習回数 */

#define LEARNING_COEFFICIENT 0.01  /* 学習係数 */
#define DELTA_INI 3.5  /* δの初期値 */
#define DELTA_FIN 0.5  /* δの最終値 */

#define DISTANCE_LOGFILE "distance.log"  /* 発火したマップ層のニューロンと入力の距離を記録するログファイルの名前 */
#define POSITION_LOGFILE "position.log"  /* 発火したマップ層のニューロンの位置を記録するログファイルの名前 */


/** 重みの初期化
 * 重みの配列を0から1の乱数で初期化する。
 *
 * weight: 初期化したい重みの配列。
 */
void init_weight(
		double weight[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH][INPUT_DATA_LENGTH]
){
	int i, j, k;

	srand(time(NULL));  /* 乱数生成器を初期化。 */

	for(i=0; i<MAP_SIDE_LENGTH; i++){
		for(j=0; j<MAP_SIDE_LENGTH; j++){
			for(k=0; k<INPUT_DATA_LENGTH; k++){
				weight[i][j][k] = (double)rand() / RAND_MAX; 
			}
		}
	}
}


/** 学習するデータを読み込む
 * 学習するデータをファイルから配列に読み込む。
 * ファイルはスペースもしくは改行区切りの数値で、小数点を許容する。
 *
 * ファイルの長さに関わらず定数で定義された長さ分だけ読み込みを行なう。
 * ファイルに記録されたデータの数が定数で定義された数より少なかった場合の動作は未定義である。
 *
 * fname: 読み込むファイルの名前。
 * data: 読み込んだデータを格納する配列。
 */
void read_data(
		const char *fname,
		double data[INPUT_DATA_NUM][INPUT_DATA_LENGTH]
){
	int k, p;
	FILE *fp;

	/* ファイル fname をオープン */
	if((fp = fopen(fname,"r")) == NULL){
		printf("Cannot open \"%s\"\n", fname);
		exit(1);
	}

	/* データの読み込み	*/
	for(p=0; p<INPUT_DATA_NUM; p++){
		for(k=0; k<INPUT_DATA_LENGTH; k++){
			fscanf(fp, "%lf ", &data[p][k]);
		}
	}

	/* ファイルをクローズ */
	fclose(fp);
}


/** 距離の計算
 * 入力と重みとの距離を計算する。
 * 距離の計算はユークリッド距離によって行なわれる。
 *
 * weight[i][j][k]: 重みベクトル。マップ層のニューロン(i,j)と入力層のニューロンkとの結合重みを表わす。
 * input: 入力データ。
 * distance: 計算した距離を出力する配列。
 */
void calc_distance(
		const double weight[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH][INPUT_DATA_LENGTH], 
		const double input[INPUT_DATA_LENGTH],
		double distance[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH]
){
	int i, j, k;

	for(i=0; i<MAP_SIDE_LENGTH; i++){
		for(j=0; j<MAP_SIDE_LENGTH; j++){
			distance[i][j] = 0;
			for(k=0; k<INPUT_DATA_LENGTH; k++){
				distance[i][j] += pow(weight[i][j][k] - input[k], 2);
			}
			distance[i][j] = sqrt(distance[i][j]);
		}
	}
}


/** 勝ちニューロンを見付ける
 * 重みと入力の距離の配列から勝ちニューロンを見付ける。
 * 勝ちニューロンは最も距離が近い（すなわち値が小さい）ことをもって決定される。
 *
 * distance: マップ層に結合する重みと入力パターンとの距離。
 * min_i: 勝ちニューロンの座標iを格納する変数へのポインタ。
 * min_j: 勝ちニューロンの座標jを格納する変数へのポインタ。
 */
void find_winner(
		double distance[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH],
		int *min_i, int *min_j
){
	int i, j;
	double min = distance[0][0];

	*min_i = *min_j = 0;

	for(i=0; i<MAP_SIDE_LENGTH; i++){
		for(j=0; j<MAP_SIDE_LENGTH; j++){
			if(min > distance[i][j]){
				*min_i = i;
				*min_j = j;
				min = distance[i][j];
			}
		}
	}
}


/** 与えられたデータを学習する
 * 与えられた入力データ群を学習し、結果を重みの配列に反映する。
 * 学習はTRAINING_NUM回行なわれる。
 *
 * weight: 重みベクトル。
 * input: 学習するデータ。
 * show_progress: 真なら計算の進捗状況を表示する。
 */
void training(
		double weight[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH][INPUT_DATA_LENGTH],
		double input[INPUT_DATA_NUM][INPUT_DATA_LENGTH],
		const int show_progress
){
	double distance[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH];	/* 距離 */
	int min_i, min_j;  /* 勝ちニューロンの座標 */
	int i, j, k;
	int t;
	int p;

	FILE *distance_log = fopen(DISTANCE_LOGFILE, "w");
	FILE *position_log = fopen(POSITION_LOGFILE, "w");

	for(t=0; t<TRAINING_NUM; t++){
		fprintf(distance_log, "%d", t);
		fprintf(position_log, "%d", t);

		for(p=0; p<INPUT_DATA_NUM; p++){
			calc_distance(  /* 距離の計算 */
				(const double (*)[MAP_SIDE_LENGTH][INPUT_DATA_LENGTH])weight,
				(const double (*))input[p],
				distance
			);
			find_winner(distance, &min_i, &min_j);  /* 勝ちニューロンを見つける */

			fprintf(distance_log, " %lf", distance[min_i][min_j]);
			fprintf(position_log, " %d %d", min_i, min_j);

			/* 重みの更新 */
			for(i=0; i<MAP_SIDE_LENGTH; i++){
				for(j=0; j<MAP_SIDE_LENGTH; j++){
					for(k=0; k<INPUT_DATA_LENGTH; k++){
						weight[i][j][k] += LEARNING_COEFFICIENT * exp(-(pow(i-min_i, 2) + pow(j-min_j, 2)) / pow(DELTA_INI * pow(DELTA_FIN/DELTA_INI, (double)t/TRAINING_NUM), 2)) * (input[p][k] - weight[i][j][k]);
					}
				}
			}
		}
		if(show_progress && t%10 == 9){
			printf("\r %d/%d", t+1, TRAINING_NUM);
			fflush(stdout);
		}
		fprintf(distance_log, "\n");
		fprintf(position_log, "\n");
	}
	fclose(distance_log);
	fclose(position_log);
	printf("\r\n");
}


/** すべての入力について計算して表示する
 * 与えられた重みを用いてすべての入力のマップ層における位置を計算し、結果を表示する。
 *
 * weight: 計算に使用する入力層からマップ層への重みの配列。
 * input: 入力するデータの配列。
 */
void calc_and_show(
		const double weight[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH][INPUT_DATA_LENGTH],
		const double input[INPUT_DATA_NUM][INPUT_DATA_LENGTH]
){
	const char *animal[INPUT_DATA_NUM + 1] = {
			"  dove","  hen ","  duck"," goose","  owl ",
			"  hawk"," eagle","  fox ","  dog ","  wolf", 
			"  cat "," tiger","  lion"," horse"," zebra",
			"  cow ","  ・  "
		};
	double distance[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH];  /* マップ層のニューロンの内部状態（入力と重みの距離） */
	int result[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH];  /* マップ層ニューロンの表すパターン */
	int min_i, min_j;  /* 勝ちニューロンの座標 */ 
	int i, j;
	int p;

	for(i=0; i<MAP_SIDE_LENGTH; i++){
		for(j=0; j<MAP_SIDE_LENGTH; j++){
			result[i][j] = INPUT_DATA_NUM;
		}
	}
	for(p=0; p<INPUT_DATA_NUM; p++){
		calc_distance(weight, input[p], distance);  /* 距離の計算 */
		find_winner(distance, &min_i, &min_j);  /* 勝ちニューロンを見つける */
		result[min_i][min_j] = p;  /* 表示用データの格納 */
	}

	/* 表示 */	 
	printf("\n");
	for(i=0; i<MAP_SIDE_LENGTH; i++){
		for(j=0; j<MAP_SIDE_LENGTH; j++){
			printf("%s", animal[result[i][j]]);
		}
		printf("\n");
	}
}


/** メイン関数
 * メイン関数。引数で学習するデータが記録されたファイルの名前を受け取り、学習前と学習後の計算結果を表示する。
 */
int main(const int argc, const char *argv[]){
	double weight[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH][INPUT_DATA_LENGTH];  /* 重み */
	double data[INPUT_DATA_NUM][INPUT_DATA_LENGTH];  /* 学習データ */

	/* 引数の数の確認 (引数の数が正しくないときは実行方法を表示) */
	if(argc <= 1){
		printf("Usage : ./a.out [TRAINING DATA] [SILENT FLAG]\n");
		printf("\n");
		printf("TRAINING DATA: training data table.\n");
		printf("SILENT FLAG: if given anything, don't show progress.\n");
		exit(1);
	}

	read_data(argv[1], data);  /* 学習データの読み込み */ 

	init_weight(weight);  /* 重みの初期化 */
	calc_and_show(  /* 学習する前の出力を計算して表示 */
		(const double (*)[MAP_SIDE_LENGTH][INPUT_DATA_LENGTH])weight,
		(const double (*)[INPUT_DATA_LENGTH])data
	);

	training(weight, data, argc <= 2);  /* 学習 */
	calc_and_show(  /* 学習後の出力を計算して表示 */
		(const double (*)[MAP_SIDE_LENGTH][INPUT_DATA_LENGTH])weight,
		(const double (*)[INPUT_DATA_LENGTH])data
	);

	return 0;
}
