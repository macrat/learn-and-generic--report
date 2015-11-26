#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define INPUT_NEURON_NUM 2  /* 入力層のニューロン数 */
#define HIDDEN_NEURON_NUM 2  /* 中間層のニューロン数 */
#define OUTPUT_NEURON_NUM 1  /* 出力層のニューロン数 */

#define LEARNING_COEFFICIENT 0.2  /* 学習係数 */
#define INPUT_PATTERN_NUM 4  /* 入力データのパターン数 */

#define TRAINING_COUNT_MAX 1000000  /* 学習回数 */
#define MINIMAL_ERROR_LEVEL 0.001  /* 許容する誤差の最大値 */

#define LOGFILE_NAME "learning.log"  /* ログファイルの名前 */


/** 出力関数（シグモイド関数）
 * 出力関数として使うシグモイド関数。
 * 引数で与えられた内部状態netの値を元にニューロンの出力値を計算する。
 *
 * net: 内部状態の値。
 *
 * return: 0から1までのニューロンの出力値。
 */
double sigmoid_func(const double net){
	return 1 / (1 + exp(-net));
}


/** 学習パターンの読み込み
 * 引数で指定されたファイルを開き、入力データと教師データを読み込む。
 * ファイルの各行が一つの入出力パターンに相当し、左からINPUT_NEURON_NUM個が入力値、残りがOUTPUT_NEURON_NUM個分の教師データである。
 *
 * 入力データは閾値の代わりに使う入力が一つ追加されるため、ファイルに記録されたデータの数よりも一つ大きい配列を用意する必要がある。
 *
 * fname: 読み込むファイルの名前。
 * input_data: 入力値を格納する配列。
 * output_data: 出力値（教師データ）を格納する配列。
 */
void read_data(
		const char *fname,
		double input_data[INPUT_PATTERN_NUM][INPUT_NEURON_NUM+1],
		double output_data[INPUT_PATTERN_NUM][OUTPUT_NEURON_NUM]
){
	int i, k, p;
	FILE *fp;

	/* ファイル fname をオープン */
	if((fp = fopen(fname,"r")) == NULL){
		printf("read_data(): Cannot open \"%s\"\n", fname);
		exit(1);
	}

	/* 学習用データの読み込み */
	for(p=0; p<INPUT_PATTERN_NUM; p++){
		for(i=0; i<INPUT_NEURON_NUM; i++){
			fscanf(fp,"%lf", &input_data[p][i]);
		}
		input_data[p][INPUT_NEURON_NUM] = 1;  /* 閾値の代わりに使うための入力。 */

		for(k=0; k<OUTPUT_NEURON_NUM; k++){
			fscanf(fp,"%lf", &output_data[p][k]);
		}
	}

	/* ファイルをクローズ */
	fclose(fp);
}


/** 重みの初期化
 * 入力層から中間層、および中間層から出力層への重みを初期化する。
 * 全ての重みは-0.5から0.5までの乱数が代入される。
 *
 * weight_i2h: 入力層からかくれ層への重みの配列。
 * weight_h2o: かくれ層から出力層への重みの配列。
 */
void init_weight(
		double weight_i2h[HIDDEN_NEURON_NUM][INPUT_NEURON_NUM+1],
		double weight_h2o[OUTPUT_NEURON_NUM][HIDDEN_NEURON_NUM+1]
){
	int i, j, k;

	srand((unsigned int)time(NULL)); /* 乱数生成器の初期化 */

	/* 入力層から中間層への重みweight_i2h[j][i]を-0.5〜0.5の乱数で初期化 */
	for(j=0; j<HIDDEN_NEURON_NUM; j++){
		for(i=0; i<INPUT_NEURON_NUM; i++){
			weight_i2h[j][i] = ((double)rand()/RAND_MAX) - 0.5;
		}
	}

	/* 中間層から出力層への重みweight_h2o[k][j]を-0.5〜0.5の乱数で初期化 */
	for(k=0; k<OUTPUT_NEURON_NUM; k++){
		for(j=0; j<HIDDEN_NEURON_NUM; j++){
			weight_h2o[k][j] = ((double)rand()/RAND_MAX) - 0.5;
		}
	}
}


/** 出力の計算（前向き計算）
 * 渡された入力値と二つの層の重みを用いてかくれ層、出力層の出力値を計算する。
 *
 * 入力の配列、二つ重みの配列とかくれ層の配列にはしきい値として使う値が含まれているため、定数で定義されたものより1つ大きなものを使用する。
 *
 * input: 入力値。
 * weight_i2h: 入力層からかくれ層への重み。
 * weight_h2o: かくれ層から出力層への重み。
 * h_out: かくれ層の出力先。
 * o_out: 出力層の出力先。
 */
void forward_propagation(
		const double input[INPUT_NEURON_NUM+1],
		const double weight_i2h[HIDDEN_NEURON_NUM][INPUT_NEURON_NUM+1],
		const double weight_h2o[OUTPUT_NEURON_NUM][HIDDEN_NEURON_NUM+1],
		double h_out[HIDDEN_NEURON_NUM+1],
		double o_out[OUTPUT_NEURON_NUM])
{
	int i, j, k;
	double h_net[HIDDEN_NEURON_NUM];  /* 中間層ニューロンの内部状態 */
	double o_net[OUTPUT_NEURON_NUM];  /* 出力層ニューロンの内部状態 */

	/* 中間層のニューロンの内部状態を計算 */
	for(j=0; j<HIDDEN_NEURON_NUM; j++){
		h_net[j] = 0;
		for(i=0; i<INPUT_NEURON_NUM+1; i++){
			h_net[j] += weight_i2h[j][i] * input[i];
		}
	}

	/* 中間層のニューロンの出力を計算 */
	for(j=0; j<HIDDEN_NEURON_NUM; j++){
		h_out[j] = sigmoid_func(h_net[j]);
	}

	h_out[HIDDEN_NEURON_NUM] = 1.0;  /* 中間層の常に1を出力するニューロンの出力を設定 (閾値の分) */

	/* 出力層のニューロンの内部状態を計算 */
	for(k=0; k<OUTPUT_NEURON_NUM; k++){
		o_net[k] = 0;
		for(j=0; j<HIDDEN_NEURON_NUM+1; j++){
			o_net[k] += weight_h2o[k][j] * h_out[j];
		}
	}

	/* 出力層のニューロンの出力を計算 */
	for(k=0; k<OUTPUT_NEURON_NUM; k++){
		o_out[k] = sigmoid_func(o_net[k]);
	}
}


/** 学習する（後ろ向き計算）
 * 入力と各層の出力、教師信号から重みを再計算（後ろ向き計算）し、学習を行なう。
 *
 * input: 入力された値。
 * h_out: かくれ層の出力。
 * o_out: 出力層の出力。
 * output: 出力されるべき値。教師信号。
 * weight_i2h: 入力層からかくれ層への重み。
 * weight_h2o: かくれ層から出力層への重み。
 */
void back_propagation(
		const double input[INPUT_NEURON_NUM+1],
		const double h_out[HIDDEN_NEURON_NUM+1],
		const double o_out[OUTPUT_NEURON_NUM],
		const double output[OUTPUT_NEURON_NUM],
		double weight_i2h[HIDDEN_NEURON_NUM][INPUT_NEURON_NUM+1],
		double weight_h2o[OUTPUT_NEURON_NUM][HIDDEN_NEURON_NUM+1]
){
	int i, j, k;

	/* 中間層から出力層への重みweight_h2o[k][j]の更新 */
	for(k=0; k<OUTPUT_NEURON_NUM; k++){
		for(j=0; j<HIDDEN_NEURON_NUM; j++){
			weight_h2o[k][j] -= LEARNING_COEFFICIENT
								* (o_out[k] - output[k]) * o_out[k]
								* (1 - o_out[k]) * h_out[j];
		}
	}

	/* 入力層から中間層への重みweight_i2h[j][i]の更新 */
	for(j=0; j<HIDDEN_NEURON_NUM; j++){
		for(i=0; i<INPUT_NEURON_NUM; i++){
			double old = weight_i2h[j][i];

			weight_i2h[j][i] = 0;
			for(k=0; k<OUTPUT_NEURON_NUM; k++){
				weight_i2h[j][i] -= (o_out[k] - output[k])
									* o_out[k] * (1 - o_out[k])
									* weight_h2o[k][j];
			}
			weight_i2h[j][i] *= LEARNING_COEFFICIENT
								* h_out[j] * (1 - h_out[j])
								* input[i];
			weight_i2h[j][i] += old;
		}
	}
}


/** メイン関数
 * 学習に使用するデータファイルの名前を引数で受け取り、誤差逆伝播法で学習、学習結果を表示する。
 */
int main(const int argc, const char *argv[]){
	double weight_i2h[HIDDEN_NEURON_NUM][INPUT_NEURON_NUM+1];  /* 入力層から中間層への重み */		
	double weight_h2o[OUTPUT_NEURON_NUM][HIDDEN_NEURON_NUM+1];  /* 中間層から出力層への重み */		
	double h_out[HIDDEN_NEURON_NUM+1];  /* 中間層ニューロンの出力 */		
	double o_out[OUTPUT_NEURON_NUM];  /* 出力層ニューロンの出力 */		
	double input[INPUT_PATTERN_NUM][INPUT_NEURON_NUM+1];  /* 入力パターン */
	double output[INPUT_PATTERN_NUM][OUTPUT_NEURON_NUM];  /* 出力パターン(教師信号) */
	double error;  /* 誤差 */ 
	FILE *log_file;  /* ファイルポインタ (誤差データの保存用) */
	int i, p, k;

	/* 引数の数の確認 (引数の数が正しくないときは実行方法を表示) */
	if(argc <= 1){
		printf("実行方法 : ./a.out [LEARNING DATA]\n");
		exit(1);
	}

	/* ログファイルをオープン */
	if((log_file = fopen(LOGFILE_NAME, "weight_h2o")) == NULL){
		printf("main(): Cannot open \"%s\"\n", LOGFILE_NAME);
		exit(1);
	}

	/* 学習データの読み込み */
	read_data(argv[1], input, output);

	/* 重みの初期化 */
	init_weight(weight_i2h, weight_h2o);

	error=20.0; /* 誤差(error)を適当な値に設定 */
	for(i=0; i<TRAINING_COUNT_MAX && error > MINIMAL_ERROR_LEVEL; i++){
		error = 0.0; 
		for(p=0; p<INPUT_PATTERN_NUM; p++){
			forward_propagation  /* 出力の計算 (前向き計算) */(
				input[p],
				(const double (*)[INPUT_NEURON_NUM+1])weight_i2h,
				(const double (*)[HIDDEN_NEURON_NUM+1])weight_h2o,
				h_out,
				o_out
			);
			back_propagation(input[p], h_out, o_out, output[p], weight_i2h, weight_h2o);  /* 出力と教師信号を元に学習 (後向き計算) */			 

			/* パターンpに対する誤差の計算 (errorに加算) */
			for(k=0; k<OUTPUT_NEURON_NUM; k++){
				error += ((o_out[k] - output[p][k]) * (o_out[k] - output[p][k])) / 2;
			}
		}
		fprintf(log_file, "%d %f\n", i, error);  /* 誤差(error)をファイルに書き込む */
	}

	fclose(log_file);  /* ログファイルを閉じる。 */


	/* 計算して結果を出力する。 */
	for(p=0; p<INPUT_PATTERN_NUM; p++){
		forward_propagation(
			input[p],
			(const double (*)[INPUT_NEURON_NUM+1])weight_i2h,
			(const double (*)[HIDDEN_NEURON_NUM+1])weight_h2o,
			h_out,
			o_out
		);

		printf("[%d] %lf %lf ===> %lf (%lf)\n",
			p,
			input[p][0], input[p][1],
			o_out[0], output[p][0]
		);
	}


	return 0;
}
