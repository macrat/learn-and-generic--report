#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GENE_LENGTH 10  /* 遺伝子の長さ（=ビット数） */

#define GENE_NUM 5  /* 一世代における遺伝子の数 */
#define MUTATION_RATE 0.1  /* 突然変異の発生確率。0で起らず、1で一世代毎に全てが反転する。 */
#define LOOP_NUM 20  /* 世代数 */

#define LOGFILE_NAME "result.log"  /* 課題用のログファイルの名前。 */


/** ランダムな遺伝子を作る
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
 *
 * genes: 計算したい遺伝子の配列。
 * fitnesses: 計算結果を保存する配列。0以上GENE_LENGTH未満の値が入る。
 */
void calc_fitnesses(const int genes[GENE_NUM][GENE_LENGTH], int fitnesses[GENE_NUM]){
	int i;

	for(i=0; i<GENE_NUM; i++){
		fitnesses[i] = calc_fitness(genes[i]);
	}
}


/** 適応度の合計を計算する
 *
 * genes: 計算したい遺伝子の配列。
 *
 * return: genesの全ての遺伝子の適応度の合計。
 */
int sum_fitness(const int genes[GENE_NUM][GENE_LENGTH]){
	int sum = 0;
	int i;

	for(i=0; i<GENE_NUM; i++){
		sum += calc_fitness(genes[i]);
	}

	return sum;
}


/** ルーレット方式で個体を一つ選ぶ
 *
 * genes: 遺伝子の一覧。この中からランダムに一つ選ぶ。
 *
 * return: 選ばれた遺伝子へのポインタ。
 */
int* choice(const int genes[GENE_NUM][GENE_LENGTH]){
	int select = rand() % sum_fitness(genes);
	int fit = 0;
	int i;

	for(i=0; i<GENE_NUM; i++){
		fit += calc_fitness(genes[i]);
		if(fit >= select){
			break;
		}
	}

	return (int*)genes[i];
}


/** 適応度が最も高い個体のインデックスを取得する
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


/** 適応度が最も低い個体のインデックスを取得する
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
 *
 * a: 一つめの親。
 * b: 二つめの親。
 * child: 生成した子供を保存する先。
 */
void cross(const int a[GENE_LENGTH], const int b[GENE_LENGTH], int child[GENE_LENGTH]){
	const int pivot = rand()%GENE_LENGTH;
	int i;

	for(i=0; i<GENE_LENGTH; i++){
		child[i] = i < pivot ? a[i] : b[i];
	}
}


/** 遺伝子をコピーする
 *
 * src: コピー元の配列。
 * dst: コピー先の配列。
 */
void copy_gene(const int src[GENE_LENGTH], int dst[GENE_LENGTH]){
	memcpy(dst, src, GENE_LENGTH * sizeof(int));
}


/** 突然変異を発生させる
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
 *
 * gene: 表示したい遺伝子。
 */
void show_gene(const int gene[GENE_LENGTH]){
	int i;

	for(i=0; i<GENE_LENGTH; i++){
		if(i == GENE_LENGTH/2){
			printf("| ");
		}
		printf("%d ", gene[i]);
	}
	printf(" (%d%%)\n", calc_fitness(gene)*100/GENE_LENGTH);
}


/** 一世代の情報を表示する
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


int main(int argc, char** argv){
	int genes[GENE_NUM][GENE_LENGTH];
	int next[GENE_NUM][GENE_LENGTH];
	int i, j;
	FILE* log_file = fopen(LOGFILE_NAME, "w");

	srand(time(NULL));  /* 乱数生成器の初期化 */

	make_genes(genes);  /* 第一世代を生成 */
	show_generation(genes);  /* 作った世代を表示する */
	printf("\n");

	fprintf(log_file, "0 %lf %lf\n", (double)sum_fitness(genes)/GENE_NUM, (double)calc_fitness(find_max_fitness(genes)));

	for(i=0; i<LOOP_NUM; i++){
		copy_gene(find_max_fitness(genes), next[0]);  /* 最も優秀な遺伝子を次の世代にコピーする。 */

		/* 次の世代の遺伝子を生成する。 */
		for(j=1; j<GENE_NUM; j++){
			cross(choice(genes), choice(genes), next[j]);
		}

		mutation(next);  /* 突然変異を起こす。 */

		memcpy(genes, next, sizeof(genes));  /* 新しい世代をコピーする。 */

		/* 新しく出来た世代の遺伝子を表示。 */
		show_generation(genes);
		printf("\n");

		fprintf(log_file, "%d %lf %lf\n", i+1, (double)sum_fitness(genes)/GENE_NUM, (double)calc_fitness(find_max_fitness(genes)));
	}

	fclose(log_file);

	return 0;
}
