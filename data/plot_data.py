#coding=UTF-8
import matplotlib.pyplot as plt
import numpy as np

# 读取数据
data = np.loadtxt('output.txt', delimiter=' ')

# 计算百分位数
percentiles = np.percentile(data, range(101))

# 绘制图像
plt.plot(percentiles)

# 添加标签和标题
plt.xlabel('百分位')
plt.ylabel('数值')
plt.title('百分位点图')

# 保存图像
plt.savefig('percentile_plot.png')

# 显示图像
plt.show()
 
