import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import confusion_matrix


data = {
    '恶寒': ['重度','轻度','无','重度','轻度','无','无','轻度','重度','无'],
    '发热': ['低热','高热','高热','低热','中热','中热','高热','低热','中热','高热'],
    '汗出': ['无汗','无汗','大汗','微汗','无汗','微汗','大汗','微汗','无汗','大汗'],
    '舌苔': ['薄白','黄腻','薄白','白腻','薄白','黄腻','黄腻','白腻','薄白','黄腻'],
    '脉象': ['浮紧','浮数','滑数','浮紧','沉细','浮数','滑数','沉细','浮紧','滑数'],
    '咽喉红肿': ['否','是','是','否','否','是','是','否','否','是'],
    '痰色': ['白稀','黄稠','黄稠','白稀','白稀','黄稠','黄稠','白稀','白稀','黄稠'],
    '证型': ['风寒束表','风热犯肺','痰热壅肺','风寒束表','阳虚外感',
           '风热犯肺','痰热壅肺','阳虚外感','风寒束表','痰热壅肺']
}
df = pd.DataFrame(data)

X = df.drop('证型', axis=1)
y = df['证型']


for col in X.columns:
    le = LabelEncoder()
    X[col] = le.fit_transform(X[col])


tree = DecisionTreeClassifier(criterion='entropy', random_state=42)
tree.fit(X, y)
pred = tree.predict(X)
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.figure(figsize=(14,8))
plot_tree(tree, feature_names=X.columns, class_names=tree.classes_, filled=True, rounded=True, fontsize=9)

plt.show()


plt.figure(figsize=(7,4))
sns.countplot(x='证型', data=df, palette='Set2')

plt.xticks(rotation=15)
plt.tight_layout()
plt.show()


plt.figure(figsize=(9,5))
imp = pd.Series(tree.feature_importances_, index=X.columns).sort_values(ascending=False)
sns.barplot(x=imp.values, y=imp.index, palette='Blues_r')
plt.xticks([0, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40])
plt.xlabel('特征重要性')
plt.ylabel('症状/体征')
plt.title('中医证型分类 ')
plt.tight_layout()
plt.show()


plt.figure(figsize=(7,5))
cm = confusion_matrix(y, pred)
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', xticklabels=tree.classes_, yticklabels=tree.classes_)
plt.xlabel('树的证型')
plt.ylabel('实际证型')

plt.tight_layout()
plt.show()