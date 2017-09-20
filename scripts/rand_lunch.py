#coding: utf-8
import random

# 随机永和大王每天吃什么
fan = ["卤肉","鸡丝","鸡丁","扣肉"
,"三杯鸡","鱼香肉丝","咖喱鸡"]
r = random.randint(0, len(fan) - 1)
print ("大王 - " + fan[r-1]).decode("utf-8").encode("gbk")

# 随机二十五块半每天吃什么
fan = [
"可乐鸡翅","叉烧肉","卤肉","扣肉"
,"三杯鸡","巴西烧肉","咖喱鸡","家乡鱼"
,"东坡肉","水煮肉片","土匪鸭","玉米排骨"
,"番茄鸡蛋","酱爆菌菇","香菇滑鸡","香辣鸡杂"
,"卤猪手","肉末茄子","剁椒鸡腿肉"]

r = random.randint(0, len(fan) - 1)
print ("二十五块半 - " + fan[r-1]).decode("utf-8").encode("gbk")
a = raw_input()