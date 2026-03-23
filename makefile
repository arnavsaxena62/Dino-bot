CXX      = g++
GADIR    = galib247
SFMLDIR  = sfml-install
CXXFLAGS = -O2 -std=c++14 -fpermissive -I$(GADIR) -I$(SFMLDIR)/include
LDFLAGS  = $(GADIR)/ga/libga.a \
           -L$(SFMLDIR)/lib \
           -lsfml-graphics -lsfml-window -lsfml-system \
           -lfreetype -ludev -lopenal -logg -lGL -lpthread \
           -Wl,-rpath,sfml-install/lib

TARGET   = my_app
SRCS 	 = main.cpp neuralnet.cpp dino.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
