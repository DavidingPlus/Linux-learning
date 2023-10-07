#定义变量
# src=add.o sub.o mult.o div.o main.o

target = a.out

allc = $(wildcard *.c)
src = $(patsubst %.c,%.o,$(allc))

# a.out:add.o sub.o mult.o div.o main.o
# 	gcc add.o sub.o mult.o div.o main.o -o a.out

$(target):$(src)
	$(CC) $(src) -o $(target)

# add.o:add.c
# 	gcc -c add.c

# sub.o:sub.c
# 	gcc -c sub.c

# mult.o:mult.c
# 	gcc -c mult.c

# div.o:div.c
# 	gcc -c div.c

# main.o:main.c
# 	gcc -c main.c

%.o:%.c
	$(CC) -c $< -o $@

#表示clean是一个伪目标，不会生成特定的文件
.PHONY:clean
clean:$(src) $(target)
	rm $(src) $(target)
