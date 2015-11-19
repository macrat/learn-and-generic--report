/********************************************************************
  自己組織化特徴マップ (サンプルプログラム)
********************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>

#define PatternNo   16     /* データ数 */
#define InputUnitNo 29     /* 入力層のニューロン数 */
#define MapUnitNo   10     /* マップ層の1辺のニューロン数 */
#define TrainingNo   2000  /* 学習回数 */

#define alpha 0.01         /* 学習係数 */
#define delta_ini 3.5      /* δの初期値 */
#define delta_fin 0.5      /* δの最終値 */

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
  重みの初期化
    引数 w[i][j][k] : 重みベクトル
                     (マップ層のニューロン(i,j)と
                                     入力層のニューロンkの結合)
********************************************************************/
void init_w(double w[MapUnitNo][MapUnitNo][InputUnitNo])
{
  int i,j,k;

  /* 乱数の初期化 */
  init_rnd();
  
  /* 重みの初期値を0〜1の乱数に設定 */

  /*** この部分を自分で書く  ***/

}

/********************************************************************
   学習データの読み込み	
     引数 fname       : ファイル名
          data[p][i]  : 学習データ (パターンpのi番目の成分) 
*********************************************************************/
void read_data(char *fname, double data[PatternNo][InputUnitNo])
{
  int k,p;
  FILE *fp;
  
  /* ファイル fname をオープン */
  if((fp=fopen(fname,"r"))==NULL){
    printf("Cannot open \"%s\"\n",fname);
    exit(1);
  }
  
  /* データの読み込み  */
  for(p=0;p<PatternNo;p++){
    for(k=0;k<InputUnitNo;k++){
      fscanf(fp,"%lf ",&data[p][k]);
    }
  }

  /* ファイルをクローズ */
  fclose(fp);
}

/********************************************************************
  学習係数αの計算
     引数 min_i : 勝ちニューロンの座標 i
          min_j : 勝ちニューロンの座標 j
          i     : ニューロンの座標 i 
          j     : ニューロンの座標 j 
          t     : 学習回数 t 
********************************************************************/
double alpha_t(int min_i, int min_j, int i, int j, double t) 
{
  /*** 学習係数 α(t)の計算 ***/

  /*** この部分を自分で書く ***/

}

/********************************************************************
  距離の計算
     引数 w[i][j][k]     : 重みベクトル
                            (マップ層のニューロン(i,j)
                                       と入力層のニューロンkの結合)
          data[p][k]     : 学習データ (パターンpのk番目の成分) 
          distance[i][j] : マップ層のニューロン(i,j)に結合する重みと
                           入力パターンとの距離
          p              : パターン番号
********************************************************************/
void calc_distance(double w[MapUnitNo][MapUnitNo][InputUnitNo], 
		   double data[PatternNo][InputUnitNo],
		   double distance[MapUnitNo][MapUnitNo],int p)
{
  int i,j,k;

  /* 入力パターンdata[p][k]とマップ層のニューロンに結合する重み
     w[i][j][k]の距離 distance[i][j]を計算する                 */ 

  /*** この部分を自分で書く ***/

}

/********************************************************************
  勝ちニューロンの決定
     引数 distance[i][j] : マップ層のニューロン(i,j)に結合する重みと
                           入力パターンとの距離
          min_i          : 勝ちニューロンの座標i 
          min_j          : 勝ちニューロンの座標j 
********************************************************************/
void find_winner(double distance[MapUnitNo][MapUnitNo], 
		 int *min_i, int *min_j)
{
  int i,j;
  double min;

  /* 入力パターンとマップ層のニューロンに結合する重みの距離
     distance[i][j] が最小となるニューロンを見つけ、i,j の値を
     *min_i, *min_j に保存                                    */ 

  /*** この部分を自分で書く ***/

}

/********************************************************************
  学習
     引数 w[i][j][k]     : 重みベクトル
                            (マップ層のニューロン(i,j)
                                       と入力層のニューロンkの結合)
          data[p][k]     : 学習データ (パターンpのk番目の成分) 
********************************************************************/
void training(double w[MapUnitNo][MapUnitNo][InputUnitNo], 
	      double data[PatternNo][InputUnitNo])
{
  double distance[MapUnitNo][MapUnitNo];  /* 距離 */
  int min_i,min_j;                        /* 勝ちニューロンの座標 */ 
  int i,j,k;
  int t;
  int p;
  
  for(t=0;t<TrainingNo;t++){
    for(p=0;p<PatternNo;p++){
      /* 距離の計算 */
      calc_distance(w,data,distance,p);
      /* 勝ちニューロンを見つける */
      find_winner(distance,&min_i,&min_j);
      /* 重みの更新 */

      /*** この部分を自分で書く ***/

    }
    fprintf(stderr,"\r %d/%d",t,TrainingNo);
  }
  printf("\n");
}

/********************************************************************
  実行
     引数 w[i][j][k]     : 重みベクトル
                            (マップ層のニューロン(i,j)
                                       と入力層のニューロンkの結合)
          data[p][k]     : 学習データ (パターンpのk番目の成分) 
********************************************************************/
void exec(double w[MapUnitNo][MapUnitNo][InputUnitNo], 
	  double data[PatternNo][InputUnitNo]
)
{
  char *animal[PatternNo+1] = {"  dove","  hen ","  duck"," goose","  owl ",
			       "  hawk"," eagle","  fox ","  dog ","  wolf", 
                               "  cat "," tiger","  lion"," horse"," zebra",
                               "  cow ","  ・  "};
  double distance[MapUnitNo][MapUnitNo]; /* 距離 */
  int result[MapUnitNo][MapUnitNo];      /* マップ層ニューロンの表すパターン */
  int min_i,min_j;                       /* 勝ちニューロンの座標 */ 
  int i,j;
  int p;

  for(i=0;i<MapUnitNo;i++){
    for(j=0;j<MapUnitNo;j++){
      result[i][j]=PatternNo;
    }
  }
  for(p=0;p<PatternNo;p++){
    /* 距離の計算 */
    calc_distance(w,data,distance,p);
    /* 勝ちニューロンを見つける */
    find_winner(distance,&min_i,&min_j);
    /* 表示用データの格納 */   
    result[min_i][min_j]=p;
  }
  /* 表示 */   
  printf("\n");
  for(i=0;i<MapUnitNo;i++){
    for(j=0;j<MapUnitNo;j++){
      printf("%s",animal[result[i][j]]);
    }
    printf("\n");
  }
}



/********************************************************************
   メインプログラム 
********************************************************************/
int main(int argc, char *argv[])
{
  double w[MapUnitNo][MapUnitNo][InputUnitNo]; /* 重み */
  double data[PatternNo][InputUnitNo];         /* 学習データ */
 

  /* 引数の数の確認 (引数の数が正しくないときは実行方法を表示) */
  if(argc!=2){
    printf("Usage : ./a.out training_data\n");
    exit(1);
  }

  read_data(argv[1],data);   /* 学習データの読み込み */ 

  init_w(w);        /* 重みの初期化 */
  exec(w,data);     /* 実行 */
  training(w,data); /* 学習 */
  exec(w,data);     /* 実行 */

  return 0;
}







