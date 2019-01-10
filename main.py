import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

pwd = os.getcwd()
main = pwd+"/Program/main.exe"  # 运行c++的编译程序
r_v = os.system(main)
print(r_v)  # 输出c++程序的运行结果
plt.figure(figsize=(10, 6.5))
plt.axis([-1, 2, -2, 4])  # 设置图像的大小和坐标轴
plt.ion()
for i in range(1, 50):
    plt.cla()
    plt.xlabel("x")
    plt.ylabel("y")
    data = []
    corx = []
    cory = []
    address1 = pwd+"/fitness/generation "+str(i)
    address2 = pwd+"/result/generation "+str(i)  # 数据存储的相对路径
    data = pd.read_csv(address1)
    with open(address2, "r") as f:  # 读取数据
        res = f.read()
    plt.title("Generation: "+str(i)+"\nMax fitness: "+res)  # 图像设置标题和显示数据
    data = data.values
    corx = data[:, 0]
    cory = data[:, 1]
    del(data)
    plt.scatter(corx, cory, color='#0000ff')  # 绘制个体的散点图
    x = np.linspace(-1, 2, 1000)
    y = [i*np.sin(i*10*np.pi)+1 for i in x]
    plt.plot(x, y, color='#5c5c5c')  # 绘制求解函数的图像
    plt.pause(0.2)
