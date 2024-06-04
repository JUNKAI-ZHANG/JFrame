# jk's Server

## 开发规范

### 分支规范
1. 不使用git merge 产生新提交
2. Dev开发，稳定版本使用Fast-Forward Merge到Master，使git历史清晰

### 代码规范

#### 变量
1. 标识
> 1. k : 自定义结构、类、函数指针......
> 2. e : 枚举
> 3. i : 整形
> 4. l : 长整型
> 5. p : 指针
> 6. s : 静态
> 7. m : 类成员
> 8. str : 字符串
> 9. map : 字典
> 10. hmap : unordered_map
> 11. hset : unordered_set

2. 声明
> 1. 禁用简写声明变量

#### 代码风格
1. 括号
> 1. 大括号在行尾

---

## 待办
1. ClientBuffer大小定多少合适？