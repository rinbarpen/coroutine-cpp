CXX = g++
CXXFLAGS = -std=c++17

# 目标文件列表
OBJS = sample1.o coroutine.o schedule.o

# 默认目标
all: sample1

lib: libcoroutine.a

# 编译目标文件
sample1.o: sample1.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ -g

coroutine.o: coroutine.cc
	$(CXX) $(CXXFLAGS) -c coroutine.cc -o coroutine.o -g

schedule.o: schedule.cc
	$(CXX) $(CXXFLAGS) -c schedule.cc -o schedule.o -g

# 构建可执行文件 sample1
sample1: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ -g3

libcoroutine.a: $(OBJS)
	ar rcs $@ $(OBJS)

# 清理生成的文件
clean:
	rm -f $(OBJS) sample1 libcoroutine.a
