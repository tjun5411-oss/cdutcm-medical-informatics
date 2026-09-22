# my_matplotlib_config.py
import matplotlib
import matplotlib.pyplot as plt
import platform

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

# 设置后端
try:
    matplotlib.use('TkAgg')
except:
    try:
        matplotlib.use('Qt5Agg')
    except:
        pass

print("Matplotlib 配置已加载 - 支持中文显示")