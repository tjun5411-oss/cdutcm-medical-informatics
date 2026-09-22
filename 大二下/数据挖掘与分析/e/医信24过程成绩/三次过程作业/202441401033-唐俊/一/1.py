import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
from scipy.stats import chi2_contingency, f_oneway
import warnings
warnings.filterwarnings('ignore')

# 设置中文字体（根据系统可用字体调整，macOS 可用 'Arial Unicode MS'）
plt.rcParams['font.family'] = 'Microsoft YaHei'  # Windows
# plt.rcParams['font.family'] = 'Arial Unicode MS'  # macOS
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['figure.dpi'] = 120

# ---------- 1. 数据加载（优先从 Excel 读取，若失败则使用硬编码数据）----------
try:
    df = pd.read_excel('data.xls', engine='xlrd')  # 若为 .xlsx 可改为 engine='openpyxl'
    print("成功从 data.xls 加载数据")
except FileNotFoundError:
    print("未找到 data.xls，使用代码内嵌数据")
    data = [
        [0.056,0.460,0.281,0.352,0.119,0.350,'S4','H4','R1','J1'],
        [0.488,0.099,0.283,0.333,0.116,0.293,'S4','H4','R1','J1'],
        [0.107,0.008,0.204,0.150,0.032,0.159,'S4','H4','R2','J2'],
        [0.322,0.208,0.305,0.130,0.184,0.317,'S4','H4','R2','J1'],
        [0.242,0.280,0.131,0.210,0.191,0.351,'S4','H4','R2R5','J1'],
        [0.389,0.112,0.456,0.277,0.185,0.396,'S4','H4','R3','J1'],
        [0.246,0.202,0.277,0.178,0.237,0.483,'S4','H4','R1R3','J3'],
        [0.330,0.125,0.356,0.268,0.366,0.397,'S4','H4','R1R2R3R5','J1'],
        [0.257,0.314,0.328,0.140,0.128,0.335,'S4','H4','R2','J2'],
        [0.205,0.330,0.253,0.295,0.115,0.224,'S4','H4','R2','J1'],
        [0.171,0.219,0.125,0.160,0.150,0.220,'S1','H1','R0','J0'],
        [0.181,0.125,0.141,0.178,0.150,0.220,'S1','H1','R0','J0'],
        [0.130,0.111,0.141,0.178,0.150,0.220,'S1','H1','R0','J0'],
    ]
    columns = [
        '肝气郁结证型系数', '热毒蕴结证型系数', '冲任失调证型系数',
        '气血两虚证型系数', '脾胃虚弱证型系数', '肝肾阴虚证型系数',
        '病程阶段', 'TNM分期', '转移部位', '确诊后几年发现转移'
    ]
    df = pd.DataFrame(data, columns=columns)

# 定义列名
numeric_cols = ['肝气郁结证型系数', '热毒蕴结证型系数', '冲任失调证型系数',
                '气血两虚证型系数', '脾胃虚弱证型系数', '肝肾阴虚证型系数']
categorical_cols = ['病程阶段', 'TNM分期', '转移部位', '确诊后几年发现转移']

# ---------- 2. 相关性分析 ----------
corr_matrix = df[numeric_cols].corr()

# ---------- 3. 分类变量间关联强度（Cramer's V 修正）----------
chi2_result = []
for i in range(len(categorical_cols)):
    for j in range(i+1, len(categorical_cols)):
        cross_tab = pd.crosstab(df[categorical_cols[i]], df[categorical_cols[j]])
        chi2, p, dof, expected = chi2_contingency(cross_tab)
        n = cross_tab.sum().sum()
        r, c = cross_tab.shape
        # 正确的 Cramer's V 公式：sqrt(chi2 / (n * min(r-1, c-1)))
        min_dim = min(r-1, c-1)
        cramers_v = np.sqrt(chi2 / (n * min_dim)) if min_dim > 0 else np.nan
        chi2_result.append([categorical_cols[i], categorical_cols[j], round(cramers_v, 3)])

# 打印 Cramer's V 结果（可选）
print("\n=== 分类变量间 Cramer's V 系数 ===")
for res in chi2_result:
    print(f"{res[0]} vs {res[1]} : {res[2]}")

# ---------- 4. 数值变量与分类变量的方差分析 ----------
anova_list = []
for n in numeric_cols:
    for c in categorical_cols:
        groups = [df[n][df[c]==g].dropna() for g in df[c].unique()]
        # 过滤掉空组
        groups = [g for g in groups if len(g) > 0]
        if len(groups) >= 2:
            f, p = f_oneway(*groups)
            if p < 0.05:
                anova_list.append([n, c, round(f, 2), round(p, 5)])

print("\n=== 方差分析显著结果 (p<0.05) ===")
for res in anova_list:
    print(f"{res[0]} ~ {res[1]} : F={res[2]}, p={res[3]}")

# ---------- 5. 可视化 ----------
import seaborn as sns

# 热力图
plt.figure(figsize=(10,8))
mask = np.triu(np.ones_like(corr_matrix, dtype=bool))
sns.heatmap(corr_matrix, mask=mask, annot=True, cmap='coolwarm', fmt='.2f',
            linewidths=0.5, square=True)
plt.title('中医证型系数相关性热力图', fontsize=14)
plt.tight_layout()
plt.show()

# 箱线图
plt.figure(figsize=(12,6))
# 使用 palette 参数（新版本 seaborn 中 color 也可用）
sns.boxplot(data=df[numeric_cols], palette='Set3')
plt.title('中医证型系数分布箱线图', fontsize=14)
plt.xlabel('证型名称', fontsize=12)
plt.ylabel('系数值', fontsize=12)
plt.xticks(rotation=15)
plt.tight_layout()
plt.show()

# 不同病程阶段的证型均值柱状图
plt.figure(figsize=(12,6))
df.groupby('病程阶段')[numeric_cols].mean().plot(kind='bar', colormap='tab10')
plt.title('不同病程阶段中医证型系数均值对比', fontsize=14)
plt.xlabel('病程阶段', fontsize=12)
plt.ylabel('平均系数值', fontsize=12)
plt.legend(bbox_to_anchor=(1.01, 1), borderaxespad=0)
plt.tight_layout()
plt.show()

# 转移部位分布柱状图
plt.figure(figsize=(10,5))
df['转移部位'].value_counts().plot(kind='bar', color='#4472C4')
plt.title('患者转移部位分布', fontsize=14)
plt.xlabel('转移部位', fontsize=12)
plt.ylabel('人数', fontsize=12)
plt.tight_layout()
plt.show()