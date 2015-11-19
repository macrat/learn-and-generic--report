/********************************************************************
   誤差逆伝播法プログラム (サンプルプログラム)
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define InputUnitNo  2   /* 入力層のニューロン数 */
#define HiddenUnitNo 2   /* 中間層のニューロン数 */
#define OutputUnitNo 1   /* 出力層のニューロン数 */

#define Eta 0.1          /* 学習係数 η */
#define PatternNo 4      /* パターン数 */

#define TrainingNo 20000  /* 学習回数 */
#define ErrorMax   0.001  /* 許容する誤差の最大値 */

/********************************************************************
  乱数の発生 (Seedの決定)
********************************************************************/
void init_rnd()
{
  srand((unsigned int)time(NULL));
}

/********************************************************************
  乱数の発生 (0〜1の乱数)
********************************************************************/
double Random()
{
  return((double)rand()/RAND_MAX);
}

/********************************************************************
  出力関数 (シグモイド関数)
    引数  net  : 内部状態の値
********************************************************************/
double SigmoidFunc(double net)
{
  /* 内部状態(net)に対するシグモイド関数の値を返す */

	return 1 / (1 + exp(-net));
}

/********************************************************************
  学習パターンの読み込み
    引数  fname            : ファイル名
          PatternIn[p][i]  : 学習パターン (入力データ)
                                    (p番目のパターンのi番目の成分)  
          PatternOut[p][k] : 学習パターン (出力データ 教師信号)
                                    (p番目のパターンのk番目の成分)  
   ファイル fname から入力データと教師信号のデータを
    PatternIn[p][i] と PatternOut[p][k]に読み込む
*********************************************************************/
void read_data(char *fname, 
	       double PatternIn[PatternNo][InputUnitNo+1],
	       double PatternOut[PatternNo][OutputUnitNo])
{
  int i,k,p;
  FILE *fp;
  
  /* ファイル fname をオープン */
  if((fp=fopen(fname,"r"))==NULL){
    printf("read_data(): Cannot open \"%s\"\n",fname);
    exit(1);
  }
  
  /* 学習用データの読み込み */
  for(p=0;p<PatternNo;p++){
    for(i=0;i<InputUnitNo;i++){
      fscanf(fp,"%lf",&PatternIn[p][i]);
    }
    for(k=0;k<OutputUnitNo;k++){
      fscanf(fp,"%lf",&PatternOut[p][k]);
    }
  }

  /* ファイルをクローズ */
  fclose(fp);
}

/********************************************************************
  重みの初期化
  引数  v[j][i] : 入力層のニューロンiから中間層のニューロンjへの重み 
        w[k][j] : 中間層のニューロンjから出力層のニューロンkへの重み 
********************************************************************/
void init_w(double v[HiddenUnitNo][InputUnitNo+1],
	    double w[OutputUnitNo][HiddenUnitNo+1])
{
  int i,j,k;

  /* 乱数の初期化 */
  init_rnd();
  
  /* 入力層から中間層への重みv[j][i]を-0.5〜0.5の乱数で初期化 */

	for(j=0; j<HiddenUnitNo; j++){
		for(i=0; i<InputUnitNo; i++){
			v[j][i] = Random() - 0.5;
		}
	}

  
  /* 中間層から出力層への重みw[k][j]を-0.5〜0.5の乱数で初期化 */

	for(k=0; k<OutputUnitNo; k++){
		for(j=0; j<HiddenUnitNo; j++){
			w[k][j] = Random() - 0.5;
		}
	}
}

/********************************************************************
  出力の計算 (前向き計算)						
    引数  p               : パターンの番号
          v[j][i]         : 入力層のニューロンiから
                                          中間層のニューロンjへの重み 
          w[k][j]         : 中間層のニューロンjから
                                          出力層のニューロンkへの重み 
	  PatternIn[p][i] : 入力パターンpのi番目の成分
	  h_out[j]        : 中間層ニューロンjの出力
	  o_out[k]        : 出力層ニューロンkの出力
********************************************************************/
void forward_propagation(int p,
			 double v[HiddenUnitNo][InputUnitNo+1],
			 double w[OutputUnitNo][HiddenUnitNo+1],
			 double PatternIn[PatternNo][InputUnitNo+1],
			 double h_out[HiddenUnitNo+1],
			 double o_out[OutputUnitNo])
{
  int i,j,k;
  double h_net[HiddenUnitNo]; /* 中間層ニューロンの内部状態 */
  double o_net[OutputUnitNo]; /* 出力層ニューロンの内部状態 */

  /* 入力層の常に1を出力するニューロンのための入力を設定 (閾値の分) */
  PatternIn[p][InputUnitNo]=1.0;

  /* 中間層のニューロンの内部状態を計算 */
  for(i=0; i<HiddenUnitNo; i++){
	  h_net[i] = 0;
	  for(j=0; j<InputUnitNo+1; j++){
		  h_net[i] += v[i][j] * PatternIn[p][j];
	  }
  }

  /* 中間層のニューロンの出力を計算 */
  for(i=0; i<HiddenUnitNo; i++){
	  h_out[i] = SigmoidFunc(h_net[i]);
  }

  /* 中間層の常に1を出力するニューロンの出力を設定 (閾値の分) */
  h_out[HiddenUnitNo]=1.0;

  /* 出力層のニューロンの内部状態を出力を計算 */
  for(i=0; i<OutputUnitNo; i++){
	  o_net[i] = 0;
	  for(j=0; j<HiddenUnitNo+1; j++){
		  o_net[i] += w[i][j] * h_out[j];
	  }
  }

  /* 出力層のニューロンの出力を計算 */
  for(i=0; i<OutputUnitNo; i++){
	  o_out[i] = SigmoidFunc(o_net[i]);
  }
}

/********************************************************************
  バックプロパゲーション (後向き計算)				
    引数  p                : パターンの番号
          v[j][i]          : 入力層のニューロンiから
                                          中間層のニューロンjへの重み 
          w[k][j]          : 中間層のニューロンjから
                                          出力層のニューロンkへの重み 
	  h_out[j]         : 中間層ニューロンjの出力
	  o_out[k]         : 出力層ニューロンkの出力
	  PatternIn[p][i]  : 入力パターンpのi番目の成分
	  PatternOut[p][k] : 出力パターンpのk番目の成分 (教師信号)
********************************************************************/
void back_propagation(int p,
		      double v[HiddenUnitNo][InputUnitNo+1],
		      double w[OutputUnitNo][HiddenUnitNo+1],
		      double h_out[HiddenUnitNo+1],
		      double o_out[OutputUnitNo],
		      double PatternIn[PatternNo][InputUnitNo+1],
		      double PatternOut[PatternNo][OutputUnitNo])
{
  int i,j,k;

  /* 中間層から出力層への重みw[k][j]の更新 */
  for(k=0; k<OutputUnitNo; k++){
	  for(j=0; j<HiddenUnitNo+1; j++){
		  w[k][j] = w[k][j] - (o_out[k] - PatternOut[p][k]) * o_out[k] * (1 - o_out[k]) * h_out[j];
	  }
  }

  /* 入力層から中間層への重みv[j][i]の更新 */
  for(j=0; j<HiddenUnitNo; j++){
	  for(i=0; i<InputUnitNo+1; i++){
		  int old = v[j][i];

		  v[j][i] = 0;
		  for(k=0; k<OutputUnitNo; k++){
			  v[j][i] += (o_out[k] - PatternOut[p][k]) * o_out[k] * (1 - o_out[k]) * w[k][j];
		  }
		  v[j][i] *= h_out[j] * (1 - h_out[j]) * PatternIn[p][i];
		  v[j][i] += old;
	  }
  }
}

/********************************************************************
  メインプログラム
********************************************************************/
int main(int argc, char *argv[])
{
  double v[HiddenUnitNo][InputUnitNo+1];  /* 入力層から中間層への重み */    
  double w[OutputUnitNo][HiddenUnitNo+1]; /* 中間層から出力層への重み */    
  double h_out[HiddenUnitNo+1];           /* 中間層ニューロンの出力 */    
  double o_out[OutputUnitNo];             /* 出力層ニューロンの出力 */    
  double PatternIn[PatternNo][InputUnitNo+1]; /* 入力パターン */
  double PatternOut[PatternNo][OutputUnitNo]; /* 出力パターン(教師信号) */
  double error; /* 誤差 */ 
  FILE *fp;    /* ファイルポインタ (誤差データの保存用) */
  int t,p,k;

  /* 引数の数の確認 (引数の数が正しくないときは実行方法を表示) */
  if(argc!=2){
    printf("実行方法 : ./a.out xor.dat\n");
    exit(1);
  }

  /* ファイル error.dat をオープン */
  if((fp=fopen("error.dat","w"))==NULL){
    printf("main(): Cannot open \"error.dat\"\n");
    exit(1);
  }

  /* 学習データの読み込み */
  read_data(argv[1],PatternIn,PatternOut);
  
  /* 重みの初期化 */
  init_w(v,w);
  
  error=20.0; /* 誤差(error)を適当な値に設定 */
  t=0;        /* 学習回数を初期化 */
  while(error>ErrorMax && t<TrainingNo){ /* 誤差がErrorMax以下になるか
                                            学習回数がTrainingNoに達するまで */
    error=0.0; 
    for(p=0;p<PatternNo;p++){ /* 全てのパターンに対して ... */
      /* 出力の計算 (前向き計算) */
      forward_propagation(p,v,w,PatternIn,h_out,o_out);
      /* バックプロパゲーション (後向き計算) */       
      back_propagation(p,v,w,h_out,o_out,PatternIn,PatternOut);
      /* パターンpに対する誤差の計算 (errorに加算) */

		for(k=0; k<OutputUnitNo; k++){
			error += ((o_out[k] - PatternOut[p][k]) * (o_out[k] - PatternOut[p][k])) / 2;
		}
    }
    t++; /* 学習回数を1進める */
    fprintf(fp,"%d %f\n",t,error); /* 誤差(error)をファイルに書き込む */
  }

  /* ファイルのクローズ */
  fclose(fp); 

  /* すべてのパターンに対し、出力の計算を行い、その結果を
         [パターンの番号] 入力データ1 入力データ2 ==> 出力 (教師信号) 
     のような形で出力する    
   (例)
     *** 結果 ***
     [0] 0.0 0.0 ==> 0.017 (0.000)
     [1] 0.0 1.0 ==> 0.971 (1.000)
     [2] 1.0 0.0 ==> 0.971 (1.000)
     [3] 1.0 1.0 ==> 0.035 (0.000)
  */

  for(p=0; p<PatternNo; p++){
	  forward_propagation(p, v, w, PatternIn, h_out, o_out);

	  printf("[%d] %lf %lf ===> %lf (%lf)\n", p, PatternIn[p][0], PatternIn[p][1], o_out[0], PatternOut[p][0]);
  }

  
  return(0);
}
