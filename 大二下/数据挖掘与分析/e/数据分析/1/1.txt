import pandas as pd
from sklearn.tree import DecisionTreeClassifier, plot_tree
import matplotlib.pyplot as plt

data = {
    "weather": ["sunny", "sunny", "overcast", "rainy", "rainy", "rainy", "overcast", "sunny", "sunny", "rainy", "sunny", "overcast", "overcast", "rainy", "overcast", "rainy", "sunny"],
    "temp": [85, 80, 83, 70, 68, 65, 64, 72, 69, 75, 75, 72, 81, 71, 85, 80, 78],
    "humidity": [85, 90, 78, 96, 80, 70, 65, 95, 70, 80, 70, 90, 75, 80, 90, 79, 70],
    "windy": ["F", "T", "F", "F", "F", "T", "T", "F", "F", "F", "T", "T", "F", "T", "F", "F", "T"],
    "play": ["No", "No", "Yes", "Yes", "Yes", "No", "Yes", "No", "Yes", "Yes", "Yes", "Yes", "Yes", "No", "?", "?", "?"]
}
df = pd.DataFrame(data)

train = df[df["play"] != "?"].copy()
unknown = df[df["play"] == "?"].copy()

weather_map = {"sunny": 0, "overcast": 1, "rainy": 2}
wind_map = {"F": 0, "T": 1}

train["weather"] = train["weather"].map(weather_map)
train["windy"] = train["windy"].map(wind_map)
unknown["weather"] = unknown["weather"].map(weather_map)
unknown["windy"] = unknown["windy"].map(wind_map)

X = train[["weather", "temp", "humidity", "windy"]]
y = train["play"]
X_pred = unknown[["weather", "temp", "humidity", "windy"]]

clf = DecisionTreeClassifier(criterion="entropy", random_state=1)
clf.fit(X, y)

preds = clf.predict(X_pred)

# 修复索引问题：同时迭代 unknown 的行和预测结果
for (idx, row), pred in zip(unknown.iterrows(), preds):
    w = list(weather_map.keys())[list(weather_map.values()).index(row["weather"])]
    wi = "有风" if row["windy"] == 1 else "没风"
    print(f"第{idx+1}天: {w}, {row['temp']}度, 湿度{row['humidity']}%, {wi} -> {'能打' if pred == 'Yes' else '别打'}")

plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.figure(figsize=(16, 10))
plot_tree(clf, feature_names=["weather", "temp", "humidity", "windy"],
          class_names=["不打", "打"], filled=True, rounded=True, fontsize=10)
plt.show()