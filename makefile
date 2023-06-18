CXX = g++
CXXFLAGS = -std=c++17

# 目标文件列表
OBJS = coroutine.o schedule.o
TARGETS = sample1 sample2 sample3
TEMP_FILES = sample1.o sample2.o sample3.o
LIBRARY = libcoroutine.a

COMPILER_FLAGS = -g

# 默认目标
all: $(TARGETS)

# 构建静态库
$(LIBRARY): $(OBJS)
	ar rcs $@ $^

# 编译目标文件
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(COMPILER_FLAGS)

# 构建可执行文件
$(TARGETS): %: %.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(COMPILER_FLAGS)

libcoroutine.a: $(OBJS)
	ar rcs $@ $(OBJS)

# 清理生成的文件
clean:
	rm -f $(TARGETS) $(OBJS) $(LIBRARY) $(TEMP_FILES)