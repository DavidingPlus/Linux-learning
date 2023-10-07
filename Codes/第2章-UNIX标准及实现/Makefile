target = app
allcpp = $(wildcard *.cpp)
src = $(patsubst %.cpp,%.o,$(allcpp))
CXX = g++

$(target) : $(src)
	$(CXX) $(src) -o $(target)

%.o:%.cpp
	$(CXX) -c $< -o $@

.PHONY:clean
clean:
	rm $(src) $(target)
