#coding=UTF-8
import matplotlib.pyplot as plt
import numpy as np

# 读取数据
data = np.loadtxt('output.txt')

# 计算百分位数
percentiles = np.percentile(data, range(99))

# 绘制图像
# plt.plot(percentiles)

# 使用字典统计每个数字出现的次数
count_dict = {}
for num in data:
    if num not in count_dict:
        count_dict[num] = 1
    else:
        count_dict[num] += 1

# 将字典转换为两个数组，一个存储数字，一个存储次数
numbers = []
counts = []
for key, value in count_dict.items():
    numbers.append(key)
    counts.append(value)

# 绘制柱状图
plt.bar(numbers, counts)

# 添加标签和标题
plt.xlabel('数字')
plt.ylabel('出现次数')
plt.title('数字出现次数统计图')

plt.xlim([0,20])

# 计算从小到大排名第 99% 的数据点的值
percentile_99 = np.percentile(data, 99)
# 在 x 轴上标注这个数据点的位置
plt.axvline(x=percentile_99, color='r', linestyle='--')
# 添加标注文本
plt.text(percentile_99, 0, ' 99%', rotation=90, va='bottom', ha='right')
# 显示图像
plt.show()
# 保存图像
plt.savefig('npercentile_plot.png')

# 显示图像

 
