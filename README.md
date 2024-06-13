# jk's Server

## 开发规范

### 分支规范
1. 不使用git merge 产生新提交
2. Dev开发，稳定版本使用Fast-Forward Merge到Master，使git历史清晰

### 指针规范
1. 禁用裸指针，一律使用智能指针

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

#### 代码风格(Google 规范)
```json
{ BasedOnStyle: Google, IndentWidth: 4, TabWidth: 4, ColumnLimit: 0, IfElseSpacing : After}
```
1. 括号
> 1. 大括号在行尾

---

## 待办
1. ~~ClientBuffer大小定多少合适？~~
2. ~~引入RouteServer~~
3. 消息分发
4. 事件系统
5. 日志写文件/引入spdlog
6. 配置文件用什么格式？并导入三方库