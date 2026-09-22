#include <stdio.h>

int main() {
    int n, i, j;
    float scores[100], sum = 0.0, avg;
    float top1 = 0, top2 = 0, top3 = 0;

    printf("请输入同学人数（n >= 10）：");
    scanf("%d", &n);

    if (n < 10) {
        printf("人数必须大于或等于10。\n");
        return 1;
    }

    printf("请输入每位同学的分数：\n");
    for (i = 0; i < n; i++) {
        scanf("%f", &scores[i]);
        sum += scores[i];
    }

    avg = sum / n;

    top1 = top2 = top3 = -1;
    for (i = 0; i < n; i++) {
        if (scores[i] > top1) {
            top3 = top2;
            top2 = top1;
            top1 = scores[i];
        } else if (scores[i] > top2) {
            top3 = top2;
            top2 = scores[i];
        } else if (scores[i] > top3) {
            top3 = scores[i];
        }
    }

    printf("所有同学的成绩平均分：%.2f\n", avg);
    printf("成绩最好的三名同学的成绩分别是：%.2f, %.2f, %.2f\n", top1, top2, top3);

    return 0;
}
