#coding: utf-8
import random

# 随机永和大王每天吃什么
fan = ["卤肉","鸡丝","鸡丁","牛柳","扣肉","三杯鸡","咕咾肉","鱼香肉丝","咖喱鸡"]

r = random.randint(0, len(fan) - 1)
print (fan[r-1]).decode("utf-8").encode("gbk")
a = raw_input()