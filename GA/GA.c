#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GENE_LENGTH 10  /* 遺伝子の長さ（=ビット数） */

#define GENE_NUM 5  /* 一世代における遺伝子の数 */
#define MUTATION_RATE 0.1  /* 突然変異の発生確率。0で起らず、1で一世代毎に全てが反転する。 */
#define LOOP_NUM 20  /* 世代数 */

#define CROSS_TYPE 1  /* 交差のタイプ。0なら一点交差、1なら二点交差、2ならランダムに交差。 */
#define CHOICE_TYPE 0  /* 選択のタイプ。0ならルーレット方式、1ならトーナメント方式。 */

#define SHOW_VERBOSE  /* これが定義されていれば計算過程を表示する。 */
/* #define STOP_WHEN_DONE */  /* これが定義されていれば最適解が出た時点で計算をやめる。 */
#define COLORFUL  /* これが定義されていれば遺伝子をカラフルに表示する。定義されていても*NIXでなければやらない。 */

#define LOGFILE_NAME "result.log"  /* 課題用のログファイルの名前。 */
#define ADVANCE_LOG_NAME "advance.log"  /* 拡張ログのファイル名。 */


/* 遺伝子の表示に使用する文字の定義。 */
#if defined(COLORFUL) \
&& (defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__))  /* COLORFULが定義されていて、かつ*NIXならカラフルな表示をする */
	#include <unistd.h>
	#define TRUE_BIT (isatty(fileno(stdout)) ? "\e[47m \e[0m" : "1 ")  /* 1の代わり */
	#define FALSE_BIT (isatty(fileno(stdout)) ? "\e[40m \e[0m" : "0 ")  /* 0の代わり */
	#define SPLIT_BIT (isatty(fileno(stdout)) ? "\e[46m \e[0m" : "| ")  /* 遺伝子の中央に表示する文字。 */
#else  /* *NIXじゃないなら普通に文字で。 */
	#undef COLORFUL
	#define TRUE_BIT "1 "
	#define FALSE_BIT "0 "
	#define SPLIT_BIT "| "
#endif


/** ランダムな遺伝子を作る
 * ランダムな遺伝子で配列を初期化する。
 *
 * genes: 初期化したい遺伝子の配列。
 */
void make_genes(int genes[GENE_NUM][GENE_LENGTH]){
	int i, j;

	/* 配列に乱数を入れていく */
	for(i=0; i<GENE_NUM; i++){
		for(j=0; j<GENE_LENGTH; j++){
			genes[i][j] = rand()%2;
		}
	}
}


/** 適応度の計算
 * 与えられた遺伝子の適応度を計算する。
 * 前半のビットが全て0、後半のビットが全て1である遺伝子の適応度をGENE_LENGTHとして、1ビット違うごとに1づつ下ってゆく。
 *
 * gene: 計算したい個体の遺伝子。0か1の値しか取らない。
 *
 * return: 計算した適応度。
 */
int calc_fitness(const int gene[GENE_LENGTH]){
	int fitness = 0;
	int i;

	for(i=0; i<GENE_LENGTH; i++){
		if(i < GENE_LENGTH/2){
			fitness += gene[i] == 0;
		}else{
			fitness += gene[i] == 1;
		}
	}

	return fitness;
}


/** 適応度をまとめて計算する
 * 遺伝子の配列について、全ての適応度をまとめて計算し、引数で与えられた配列に結果を代入する。
 *
 * genes: 計算したい遺伝子の配列。
 * fitnesses: 計算結果を保存する配列。0以上GENE_LENGTH未満の値が入る。
 */
void calc_fitness_list(
		const int genes[GENE_NUM][GENE_LENGTH],
		int fitnesses[GENE_NUM]
){
	int i;

	for(i=0; i<GENE_NUM; i++){
		fitnesses[i] = calc_fitness(genes[i]);
	}
}


/** 適応度の合計を計算する
 * 配列で渡されたすべての遺伝子の適応度の合計を計算する。
 *
 * genes: 計算したい遺伝子の配列。
 *
 * return: 引数genesの全ての遺伝子の適応度の合計。
 */
int sum_fitness(const int genes[GENE_NUM][GENE_LENGTH]){
	int sum = 0;
	int i;

	for(i=0; i<GENE_NUM; i++){
		sum += calc_fitness(genes[i]);
	}

	return sum;
}


/** 交差に使う個体をランダムに一つ選ぶ
 * 交差に使用する個体をランダムに一つ選ぶ。
 * 選択の方法はCHOICE_TYPE定数によって決まる。
 *
 * genes: 遺伝子の一覧。この中からランダムに一つ選ぶ。
 *
 * return: 選ばれた遺伝子へのポインタ。
 */
int* choice(const int genes[GENE_NUM][GENE_LENGTH]){
#if CHOICE_TYPE == 0
	const int select = rand() % sum_fitness(genes);
	int fit = 0;
	int i;

	for(i=0; i<GENE_NUM; i++){
		fit += calc_fitness(genes[i]);
		if(fit >= select){
			break;
		}
	}

	return (int*)genes[i];
#else
	const int a = rand()%GENE_NUM;  /* 一つめの候補は適当に決める。 */
	int b;

	/* 二つめの候補は一つめと被らないように決める。 */
	do{
		b = rand()%GENE_NUM;
	}while(a == b);

	/* 適応度の高い方を選ぶ。 */
	if(calc_fitness(genes[a]) > calc_fitness(genes[b])){
		return (int*)genes[a];
	}else{
		return (int*)genes[b];
	}
#endif
}


/** 適応度が最も高い個体を探す
 * 与えられた遺伝子の配列のうち、適応度が最も高い個体を探す。
 *
 * genes: 探したい遺伝子の一覧。
 *
 * return: 渡された遺伝子の中で最も適応度が高い遺伝子へのポインタ。
 */
int* find_max_fitness(const int genes[GENE_NUM][GENE_LENGTH]){
	int i, max_fit=0, max_idx;

	for(i=0; i<GENE_NUM; i++){
		int fit = calc_fitness(genes[i]);
		if(fit > max_fit){
			max_fit = fit;
			max_idx = i;
		}
	}

	return (int*)genes[max_idx];
}


/** 適応度が最も低い個体を探す
 * 与えられた遺伝子の配列のうち、適応度が最も低い個体を探す。
 *
 * genes: 探したい遺伝子の一覧。
 *
 * return: 渡された遺伝子の中で最も適応度が低い遺伝子へのポインタ。
 */
int* find_min_fitness(const int genes[GENE_NUM][GENE_LENGTH]){
	int i, min_fit=GENE_LENGTH, min_idx;

	for(i=0; i<GENE_NUM; i++){
		int fit = calc_fitness(genes[i]);
		if(fit < min_fit){
			min_fit = fit;
			min_idx = i;
		}
	}

	return (int*)genes[min_idx];
}


/** 二つの遺伝子を交差させて新たな遺伝子を作る
 * 引数で与えられた二つの遺伝子を交差させて新たな遺伝子を作り、引数childに格納する。
 * 交差の方法は定数CROSS_TYPEによって決定される。
 *
 * a: 一つめの親。
 * b: 二つめの親。
 * child: 生成した子供を保存する先。
 */
void cross(
		const int a[GENE_LENGTH],
		const int b[GENE_LENGTH],
		int child[GENE_LENGTH]
){
	int i;
#if CROSS_TYPE == 0
	const int pivot = rand()%(GENE_LENGTH-2) + 1;

	for(i=0; i<GENE_LENGTH; i++){
		child[i] = i < pivot ? a[i] : b[i];
	}
#elif CROSS_TYPE == 1
	const int pivot_b = rand()%(GENE_LENGTH-3) + 2;
	const int pivot_a = rand()%pivot_b + 1;

	for(i=0; i<GENE_LENGTH; i++){
		child[i] = i < pivot_a || pivot_b < i ? a[i] : b[i];
	}
#else
	for(i=0; i<GENE_LENGTH; i++){
		child[i] = rand()%2 ? a[i] : b[i];
	}
#endif
}


/** 突然変異を発生させる
 * 与えられた遺伝子の配列全体について、定数MUTATION_RATEの確率で突然変異を起こす。
 *
 * genes: 突然変異を起こしたい遺伝子の配列。
 */
void mutation(int genes[GENE_NUM][GENE_LENGTH]){
	int i, j;

	for(i=0; i<GENE_NUM; i++){
		for(j=0; j<GENE_LENGTH; j++){
			if(MUTATION_RATE > (double)rand()/RAND_MAX){
				genes[i][j] = !genes[i][j];
			}
		}
	}
}


/** 遺伝子情報を表示する
 * 一つの遺伝子の情報を表示する。
 * 表示に使用する文字は定数TRUE_BITとFALSE_BITによって定義される。
 * また、遺伝子の中央にはSPLIT_BITが表示される。
 * いずれの定数も文字列で、任意の長さを設定出来る。
 *
 * gene: 表示したい遺伝子。
 */
void show_gene(const int gene[GENE_LENGTH]){
	int i;

	for(i=0; i<GENE_LENGTH; i++){
		if(i == GENE_LENGTH/2){
			printf(SPLIT_BIT);
		}
		printf("%s", gene[i] ? TRUE_BIT : FALSE_BIT);
	}
	printf(" (%d%%)\n", calc_fitness(gene)*100/GENE_LENGTH);
}


/** 一世代の情報を表示する
 * 一世代全ての遺伝子の情報を表示する。
 * show_gene関数で表示される情報に加え、その世代における最大、最小、平均などの値も表示される。
 *
 * genes: 表示したい遺伝子の配列。
 */
void show_generation(const int genes[GENE_NUM][GENE_LENGTH]){
	int i;

	for(i=0; i<GENE_NUM; i++){
		show_gene(genes[i]);
	}

	printf("max: %d%%\n", calc_fitness(find_max_fitness(genes))*100/GENE_LENGTH);
	printf("min: %d%%\n", calc_fitness(find_min_fitness(genes))*100/GENE_LENGTH);
	printf("average: %d%%\n", sum_fitness(genes)*100/GENE_NUM/GENE_LENGTH);
}


/** ソート用の比較関数
 * int配列のソートに使用するための比較関数。
 */
int sort_cmp(const int* a, const int* b){
	return *b - *a;
}


/** ログファイルに一世代分の情報を追記する
 * 二種類のログファイルに一世代分の情報を追記する。
 *
 * normalログには講義で指示された基本的な内容が記録される。
 * advanceログにはその世代における最大、最小、平均、中央値が記録される。
 *
 * 内部で呼び出された回数を数えており、normalログへの記録に使用している。
 *
 * normal: 課題用のログファイルへのファイルポインタ。
 * advance: 拡張ログファイルへのファイルポインタ。
 * genes: 記録したい世代の遺伝子の配列。
 */
void write_log(
		FILE* normal,
		FILE* advance,
		const int genes[GENE_NUM][GENE_LENGTH]
){
	int fitnesses[GENE_NUM];
	double avg;

	static int count = 0;
	count++;


	calc_fitness_list((const int (*)[GENE_LENGTH])genes, fitnesses);
	qsort(
		fitnesses,
		GENE_NUM,
		sizeof(int),
		(int (*)(const void*, const void*))sort_cmp
	);

	avg = (double)sum_fitness((const int (*)[GENE_LENGTH])genes)/GENE_NUM;

	fprintf(normal, "%d %lf %d\n", count, avg, fitnesses[0]);
	fprintf(
		advance,
		"%lf %lf %lf %lf\n",
		avg/GENE_LENGTH,
		(double)fitnesses[0]/GENE_LENGTH,
		(double)fitnesses[GENE_NUM-1]/GENE_LENGTH,
		(double)fitnesses[GENE_NUM/2]/GENE_LENGTH
	);
}


/** メイン関数
 * メイン関数。引数は受けとらない。
 * 
 * 遺伝子の生成や計算、表示などの関数の呼び出しを行なう。
 * 計算はLOOP_NUM世代繰り返して行なわれる。
 */
int main(const int argc, const char* argv[]){
	int genes[GENE_NUM][GENE_LENGTH];
	int next[GENE_NUM][GENE_LENGTH];
	int i, j;
	FILE* log_file = fopen(LOGFILE_NAME, "w");
	FILE* adv_log_file = fopen(ADVANCE_LOG_NAME, "w");

	srand(time(NULL));  /* 乱数生成器の初期化 */

	make_genes(genes);  /* 第一世代を生成 */
	show_generation((const int (*)[GENE_LENGTH])genes);  /* 作った世代を表示する */
	printf("\n");

	write_log(log_file, adv_log_file, (const int (*)[GENE_LENGTH])genes);

#ifdef STOP_WHEN_DONE
	for(i=0; i<LOOP_NUM && calc_fitness(find_max_fitness(genes))<GENE_LENGTH; i++){
#else
	for(i=0; i<LOOP_NUM; i++){
#endif
		/* 次の世代の遺伝子を生成する。 */
		for(j=1; j<GENE_NUM; j++){
			cross(
				choice((const int (*)[GENE_LENGTH])genes),
				choice((const int (*)[GENE_LENGTH])genes),
				next[j]
			);
		}

		mutation(next);  /* 突然変異を起こす。 */

		memcpy(next[0], find_max_fitness((const int (*)[GENE_LENGTH])genes), GENE_LENGTH * sizeof(int));  /* 最も優秀な遺伝子を次の世代にコピーする。 */

		memcpy(genes, next, sizeof(genes));  /* 新しい世代をコピーする。 */

#ifdef SHOW_VERBOSE
		/* 新しく出来た世代の遺伝子を表示。 */
		show_generation((const int (*)[GENE_LENGTH])genes);
		printf("\n");
#endif

		write_log(log_file, adv_log_file, (const int (*)[GENE_LENGTH])genes);
	}

#ifndef SHOW_VERBOSE
	/* 計算結果を表示。 */
	show_generation((const int (*)[GENE_LENGTH])genes);
#endif

	fclose(log_file);
	fclose(adv_log_file);

	return 0;
}
