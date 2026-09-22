import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score, calinski_harabasz_score, adjusted_rand_score, normalized_mutual_info_score
from sklearn.decomposition import PCA

# 原始数据（中医症状打分）
raw = [
    [1,3,2,3,0,0,1,2,3],
    [2,2,3,2,0,0,1,2,3],
    [3,3,2,2,1,0,1,3,2],
    [4,3,3,3,0,0,0,2,3],
    [5,2,2,3,1,0,1,2,2],
    [6,3,1,2,0,0,1,3,2],
    [7,2,3,3,0,0,0,2,3],
    [8,3,2,3,1,0,1,2,2],
    [9,2,2,2,0,0,1,3,3],
    [10,3,3,2,0,1,1,2,3],
    [11,2,2,3,0,0,2,3,2],
    [12,3,2,2,0,0,1,2,3],
    [13,2,3,2,1,0,1,2,2],
    [14,3,2,3,0,0,0,3,3],
    [15,2,2,2,0,1,1,2,2],
    [16,3,3,3,1,0,1,3,3],
    [17,0,0,0,3,3,2,0,0],
    [18,0,1,0,3,2,2,1,0],
    [19,0,0,1,2,3,2,0,0],
    [20,1,0,0,3,3,1,0,0],
    [21,0,0,0,2,3,2,1,0],
    [22,0,1,1,3,2,1,0,1],
    [23,1,0,0,3,3,2,0,0],
    [24,0,0,0,2,2,3,1,0],
    [25,0,1,0,3,3,2,0,0],
    [26,1,0,1,2,3,1,0,0],
    [27,0,0,0,3,2,2,1,0],
    [28,0,1,0,2,3,2,0,1],
    [29,1,0,0,3,3,1,1,0],
    [30,0,0,1,3,2,2,0,0],
    [31,0,1,0,2,3,3,1,0],
    [32,0,0,0,3,3,2,0,1],
    [33,1,0,0,2,1,3,3,2],
    [34,0,0,0,2,1,3,3,1],
    [35,1,1,0,2,0,3,2,2],
    [36,0,0,1,3,1,3,3,1],
    [37,1,0,0,2,1,2,3,2],
    [38,0,1,0,2,0,3,2,1],
    [39,1,0,1,3,1,3,3,2],
    [40,0,0,0,2,1,2,3,1],
    [41,1,1,0,2,0,3,3,2],
    [42,0,0,1,3,1,3,2,1],
    [43,1,0,0,2,1,3,3,1],
    [44,0,1,0,2,0,2,3,2],
    [45,1,0,1,3,1,3,3,1],
    [46,0,0,0,2,1,3,2,2],
    [47,1,1,0,2,0,3,3,1],
    [48,0,0,1,3,1,2,3,2],
    [49,1,0,0,2,1,3,2,1],
    [50,0,1,0,3,0,3,3,2]
]
col_names = ["编号","口苦","胁痛","烦躁","乏力","畏寒","纳呆","苔腻","脉弦滑"]
df = pd.DataFrame(raw, columns=col_names)

# 真实分组（前16个一类，中间16个二类，后18个三类）
real_group = [0]*16 + [1]*16 + [2]*18
df["真实分组"] = real_group

# 特征列
feats = ["口苦","胁痛","烦躁","乏力","畏寒","纳呆","苔腻","脉弦滑"]
X = df[feats].values

# 标准化
scaler = StandardScaler()
X_norm = scaler.fit_transform(X)

# K=3跑一下
k = 3
kmeans = KMeans(n_clusters=k, random_state=42)
df["聚类分组"] = kmeans.fit_predict(X_norm)

# 内部指标
sil = silhouette_score(X_norm, df["聚类分组"])
ch = calinski_harabasz_score(X_norm, df["聚类分组"])

# 外部指标（跟真实分组比）
ari = adjusted_rand_score(df["真实分组"], df["聚类分组"])
nmi = normalized_mutual_info_score(df["真实分组"], df["聚类分组"])

print("="*60)
print(f"轮廓系数: {sil:.3f}  ")
print(f"CH指数: {ch:.2f}  ")
print("-"*60)
print(f"ARI: {ari:.3f}")
print(f"NMI: {nmi:.3f} ")
print("="*60)

# 可视化：PCA降维后画点
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

pca = PCA(n_components=2)
X_2d = pca.fit_transform(X_norm)

plt.figure(figsize=(10,6))
colors = ["red", "blue", "green"]
for i in range(k):
    mask = df["聚类分组"] == i
    plt.scatter(X_2d[mask, 0], X_2d[mask, 1], c=colors[i], label=f"第{i}类", s=70, alpha=0.7)

plt.title("K-Means的三类")
plt.xlabel("主成分1")
plt.ylabel("主成分2")
plt.legend()
plt.grid(alpha=0.3)
plt.show()