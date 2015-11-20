/********************************************************************
	自己組織化特徴マップ (サンプルプログラム)
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define PatternNo 16  /* データ数 */
#define InputUnitNo 29  /* 入力層のニューロン数 */
#define MapUnitNo 10  /* マップ層の1辺のニューロン数 */
#define TrainingNo 2000  /* 学習回数 */

#define alpha 0.01  /* 学習係数 */
#define delta_ini 3.5  /* δの初期値 */
#define delta_fin 0.5  /* δの最終値 */


/** 重みの初期化
 * 重みの配列を0から1の乱数で初期化する。
 *
 * weight[i][j][k]: 初期化したい重みの配列。。
 */
void init_w(double weight[MapUnitNo][MapUnitNo][InputUnitNo]){
	int i, j, k;

	srand((unsigned int)time(NULL));  /* 乱数生成器を初期化。 */
	
	for(i=0; i<MapUnitNo; i++){
		for(j=0; j<MapUnitNo; j++){
			for(k=0; k<MapUnitNo; k++){
				weight[i][j][k] = (double)rand() / RAND_MAX; 
			}
		}
	}
}


/** 学習するデータを読み込む
 *
 * fname: 読み込むファイルの名前。
 * data: 読み込んだデータを格納する配列。
 */
void read_data(char *fname, double data[PatternNo][InputUnitNo])
{
	int k, p;
	FILE *fp;
	
	/* ファイル fname をオープン */
	if((fp = fopen(fname,"r")) == NULL){
		printf("Cannot open \"%s\"\n", fname);
		exit(1);
	}
	
	/* データの読み込み	*/
	for(p=0; p<PatternNo; p++){
		for(k=0; k<InputUnitNo; k++){
			fscanf(fp, "%lf ", &data[p][k]);
		}
	}

	/* ファイルをクローズ */
	fclose(fp);
}


/** 距離の計算
 * 入力と重みとの距離を計算する。
 *
 * weight[i][j][k]: 重みベクトル。マップ層のニューロン(i,j)と入力層のニューロンkとの結合重みを表わす。
 * input: 入力データ。
 * distance: 計算した距離を出力する配列。
 */
void calc_distance(double weight[MapUnitNo][MapUnitNo][InputUnitNo], 
					double input[InputUnitNo],
					double distance[MapUnitNo][MapUnitNo])
{
	int i, j, k;

	for(i=0; i<MapUnitNo; i++){
		for(j=0; j<MapUnitNo; j++){
			distance[i][j] = 0;
			for(k=0; k<MapUnitNo; k++){
				distance[i][j] += pow(weight[i][j][k] - input[k], 2);
			}
			distance[i][j] = sqrt(distance[i][j]);
		}
	}
}


/** 勝ちニューロンを見付ける
 *
 * distance: マップ層に結合する重みと入力パターンとの距離。
 * min_i: 勝ちニューロンの座標iを格納する変数へのポインタ。
 * min_j: 勝ちニューロンの座標jを格納する変数へのポインタ。
 */
void find_winner(double distance[MapUnitNo][MapUnitNo], int *min_i, int *min_j){
	int i, j;
	double min = distance[0][0];

	*min_i = *min_j = 0;

	for(i=0; i<MapUnitNo; i++){
		for(j=0; j<MapUnitNo; j++){
			if(min > distance[i][j]){
				*min_i = i;
				*min_j = j;
				min = distance[i][j];
			}
		}
	}
}


/** 与えられたデータを学習する
 *
 * weight: 重みベクトル。
 * input: 学習するデータ。
 */
void training(double weight[MapUnitNo][MapUnitNo][InputUnitNo], double input[PatternNo][InputUnitNo]){
	double distance[MapUnitNo][MapUnitNo];	/* 距離 */
	int min_i, min_j;  /* 勝ちニューロンの座標 */
	int i, j, k;
	int t;
	int p;
	
	for(t=0; t<TrainingNo; t++){
		for(p=0; p<PatternNo; p++){
			calc_distance(weight, input[p], distance);  /* 距離の計算 */
			find_winner(distance, &min_i, &min_j);  /* 勝ちニューロンを見つける */

			/* 重みの更新 */
			for(i=0; i<MapUnitNo; i++){
				for(j=0; j<MapUnitNo; j++){
					for(k=0; k<InputUnitNo; k++){
						weight[i][j][k] += alpha * exp(-(pow(i-min_i, 2) + pow(j-min_j, 2)) / pow(delta_ini * pow(delta_fin/delta_ini, (double)t/TrainingNo), 2)) * (input[p][k] - weight[i][j][k]);
					}
				}
			}
		}
		if(t%10 == 9){
			printf("\r %d/%d", t+1, TrainingNo);
			fflush(stdout);
		}
	}
	printf("\r\n");
}

/********************************************************************
	実行
		 引数 w[i][j][k]		 : 重みベクトル (マップ層のニューロン(i,j)と入力層のニューロンkの結合)
				data[p][k]		 : 学習データ (パターンpのk番目の成分) 
********************************************************************/
void exec(double w[MapUnitNo][MapUnitNo][InputUnitNo], double data[PatternNo][InputUnitNo]){
	char *animal[PatternNo+1] = {
			"  dove","  hen ","  duck"," goose","  owl ",
			"  hawk"," eagle","  fox ","  dog ","  wolf", 
			"  cat "," tiger","  lion"," horse"," zebra",
			"  cow ","  ・  "
		};
	double distance[MapUnitNo][MapUnitNo]; /* 距離 */
	int result[MapUnitNo][MapUnitNo];  /* マップ層ニューロンの表すパターン */
	int min_i, min_j;  /* 勝ちニューロンの座標 */ 
	int i, j;
	int p;

	for(i=0; i<MapUnitNo; i++){
		for(j=0; j<MapUnitNo; j++){
			result[i][j] = PatternNo;
		}
	}
	for(p=0; p<PatternNo; p++){
		calc_distance(w, data[p], distance);  /* 距離の計算 */
		find_winner(distance, &min_i, &min_j);  /* 勝ちニューロンを見つける */
		result[min_i][min_j] = p;  /* 表示用データの格納 */
	}

	/* 表示 */	 
	printf("\n");
	for(i=0; i<MapUnitNo; i++){
		for(j=0; j<MapUnitNo; j++){
			printf("%s", animal[result[i][j]]);
		}
		printf("\n");
	}
}


/********************************************************************
	 メインプログラム 
********************************************************************/
int main(int argc, char *argv[])
{
	double w[MapUnitNo][MapUnitNo][InputUnitNo];  /* 重み */
	double data[PatternNo][InputUnitNo];  /* 学習データ */
 

	/* 引数の数の確認 (引数の数が正しくないときは実行方法を表示) */
	if(argc <= 1){
		printf("Usage : ./a.out training_data\n");
		exit(1);
	}

	read_data(argv[1],data);  /* 学習データの読み込み */ 

	init_w(w);  /* 重みの初期化 */
	exec(w, data);  /* 実行 */
	training(w, data);  /* 学習 */
	exec(w, data);  /* 実行 */

	return 0;
}
